//
// Created by shooting_star_t33 on 9/24/24.
//

#include "Client.hpp"

namespace client {
        Client::Client(asio::io_context& io_context, const std::string& server_ip, short server_port)
            :   io_context_(io_context),
                socket_(io_context, udp::endpoint(udp::v4(), 0)),
                remote_endpoint_(asio::ip::address::from_string(server_ip), server_port),
                is_running_(false) {

            receive_thread_ = std::thread(&Client::receive_loop, this);
            main_loop();
        }

        Client::~Client() {
            is_running_ = false;  // Arrête la boucle de réception
            if (receive_thread_.joinable()) {
                receive_thread_.join();  // Attends que le thread termine
            }
        }

        void Client::send_message(const std::string &message) {
            socket_.send_to(asio::buffer(message), remote_endpoint_);
        }

        void Client::send_message_to_player(const std::string& message, const udp::endpoint& player_endpoint) {
            socket_.send_to(asio::buffer(message), player_endpoint);
        }

        void Client::main_loop() {
            std::string input;
            while (!is_running_) {
                std::cout << "Enter message to send (or type 'exit' to quit): ";
                std::getline(std::cin, input);

                if (input == "exit") {
                    is_running_ = false;
                    break;
                }

                send_message(input);
            }
        }

        void Client::send_packet(Packet& packet) const {
            packet.format_data();
            packet.send_packet(remote_endpoint_);
        }

        void Client::receive_loop() {
            start_receive();
            io_context_.run();
        }

        void Client::fillCommandsToSends(std::string command) {
            _commandsToSend[command] = command;
        }

        void Client::start_receive() {
            socket_.async_receive_from(
            asio::buffer(recv_buffer_), remote_endpoint_,
            [this](std::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                    handle_receive(bytes_recvd);
                } else {
                    start_receive();
                }
            });
        }

        void Client::manage_message(std::size_t bytes_transferred) {
            std::cout << "Manage message: " << bytes_transferred << std::endl;
            std::string message = std::string(recv_buffer_.data(), bytes_transferred);
            std::cout << "Message: " << recv_buffer_.data() << std::endl;
            if (!im_host) {
                if (bytes_transferred == 3) {
                    if (recv_buffer_[2] == 0) {
                        std::cout << "Received: PING" << std::endl;
                        for (size_t i = 0; i < bytes_transferred; ++i) {
                            std::cout << static_cast<int>(recv_buffer_[i]) << "|";
                        }
                        auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::PING, socket_);
                        send_packet(*packet);
                    return ;
                    }
                }
                if (message.find("GAME_LAUNCH|HOST") == 0) {
                    std::cout << "Received: GAME_LAUNCH|HOST" << std::endl;
                    im_host = true;
                    command_handler_ = std::make_unique<CommandHandler>(*this);
                    return ;
                }
                std::cout << "The message is : " << message << std::endl;
                if (message.find("GAME_LAUNCH|") == 0) {
                    std::cout << "Received: GAME_LAUNCH" << std::endl;
                    std::cout << message << std::endl;
                    std::string host_data = message.substr(message.find('|') + 1);
                    std::string host_ip = host_data.substr(0, host_data.find(':'));
                    std::string host_port = host_data.substr(host_data.find(':') + 1, host_data.rfind(':') - host_data.find(':') - 1);
                    std::string host_id = host_data.substr(host_data.rfind(':') + 1);
                    remote_endpoint_ = udp::endpoint(asio::ip::address::from_string(host_ip), std::stoi(host_port));
                    send_message("GAME_LAUNCH|ACK");
                }
            } else {
                std::cout << "Received As Host: " << message << std::endl;
                if (message.find("GAME_LAUNCH|ACK") == 0) {
                    players_endpoints_[std::to_string(number_of_players_)] = remote_endpoint_;
                    number_of_players_ += 1;
                    send_message_to_player("Start the game", remote_endpoint_);
                    return ;
                }
                if (message.find("command|") == 0) {
                    std::string command = message.substr(message.find('|') + 1);
                    command_handler_.get()->addCommand(command);
                    return ;
                }
            }
            std::cout << std::endl;
        }

        void Client::handle_receive(std::size_t bytes_transferred) {
            std::cout << "Handle receive: " << bytes_transferred << std::endl;
            if (bytes_transferred > 0) {
                std::lock_guard<std::mutex> lock(messages_mutex_);
                recv_buffer_size_ = bytes_transferred;
                manage_message(bytes_transferred);

                auto message = std::string(recv_buffer_.data(), bytes_transferred);
                //std::cout << "Received message: " << message << std::endl;

                received_messages_[message_id_counter_] = std::make_pair(message, remote_endpoint_);

                // Increment the message ID
                message_id_counter_ += 1;
                std::cout << "Message ID: " << message_id_counter_ << std::endl;
                // Notify the server loop that a message has been received
                std::fill(recv_buffer_.begin() + bytes_transferred, recv_buffer_.end(), 0);
                messages_condition_.notify_one();
            }
            start_receive();
        }

        void Client::insertCommands(const std::map<std::string, std::string>& commands) {
            for (const auto& command : commands) {
                _commandsToDo[command.first] = command.second;
            }
        }
}