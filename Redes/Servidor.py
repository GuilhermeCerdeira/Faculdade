# Server Script
import socket
import ipaddress
from colorama import Fore, init

init(autoreset=True)

LOGIN = "usuario123"
HOST = "127.0.0.1"
PORT = 5000


def validate_login(received_login):
    return received_login == LOGIN


def calculate_network_info(ip_network, ip_version):

    try:
        network = ipaddress.ip_network(ip_network, strict=False)

        if ip_version == 4:
            total_hosts = network.num_addresses
            first_host = network.network_address + 1
            last_host = network.broadcast_address - 1
        else:
            total_hosts = network.num_addresses
            first_host = network.network_address + 1
            last_host = network.broadcast_address - 1

        return total_hosts, first_host, last_host
    except ValueError as e:
        print(Fore.RED + f"Endereço de IP inválido: {e}")
        return None


def start_server():
    tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    tcp.bind((HOST, PORT))
    tcp.listen()
    connection, client = tcp.accept()
    print(f"\nConexão de: {client}")
    login_attempt = connection.recv(1024).decode()
    if not validate_login(login_attempt):
        print(Fore.RED + "Login incorreto")
        connection.close()
        return
    print(Fore.GREEN + "\nUsuário logado")

    while True:

        try:
            ip_version = int(connection.recv(1024).decode())
            ip_network = connection.recv(1024).decode()
            result = calculate_network_info(ip_network, ip_version)

            if result:
                response = ",".join(map(str, result))
                connection.send(response.encode())
            else:
                connection.send("Endereço de ip inválido".encode())

        except Exception as e:
            print(e)
            connection.close()
            break


if __name__ == "__main__":
    start_server()
