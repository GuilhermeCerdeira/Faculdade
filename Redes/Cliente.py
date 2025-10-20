import socket
from colorama import Fore, init

init(autoreset=True)

HOST = '127.0.0.1'  
PORT = 5000 

def start_client():
    tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        tcp.connect((HOST, PORT))
        print('\nInsira suas mensagens')
        print('Para sair digite: EXIT\n')
        
        login = input("Insira o login: ")
        tcp.send(login.encode())
        
        while True:
            print(Fore.YELLOW + "\nInsira\n1- para IPV4\n2- para IPV6 \ndigite 0 para sair")
            
            ip_version = input()

            if ip_version == '0':
                break

            tcp.send(ip_version.encode())
            
            print(Fore.YELLOW + "\nInsira o endereço de IP:")
            ip_network = input()
            tcp.send(ip_network.encode())
            
            try:
                network_info = tcp.recv(4096).decode()
                
                if network_info.startswith("Error"):
                    print(Fore.RED + network_info)
                    continue
                
                info = network_info.split(',')
                print(Fore.GREEN + "\n-----------------------------")
                print(Fore.GREEN + f"\nQuantidade de máquinas disponíveis: {info[0]}")
                print(Fore.GREEN + f"Primeiro endereço útil: {info[1]}")
                print(Fore.GREEN + f"Último endereço útil: {info[2]}")
                print(Fore.GREEN + "-----------------------------\n")
            
            except Exception as e:
                print(Fore.RED + f"Erro: {e}")
            
            cont = input("Deseja continuar? (s/n): ").lower()
            if cont == 'n':
                break
    
    except:
        tcp.close()

if __name__ == "__main__":
    start_client()