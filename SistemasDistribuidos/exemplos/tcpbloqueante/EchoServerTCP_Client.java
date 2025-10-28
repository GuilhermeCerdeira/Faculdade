// CLIENTE
import java.io.*;
import java.net.*;
// import com.google.gson.Gson;
// import com.google.gson.GsonBuilder;

public class EchoServerTCP_Client {
    public static void main(String[] args) {
        Socket clientSocket = null;
        DataInputStream in = null;
        PrintStream out = null;
        BufferedReader teclado = new BufferedReader(new InputStreamReader(System.in));

        try {
            System.out.print("Qual o IP do servidor? ");
            String serverIP = teclado.readLine();

            System.out.print("Qual a Porta do servidor? ");
            int serverPort = Integer.parseInt(teclado.readLine());

            System.out.println("Tentando conectar com host " + serverIP + " na porta " + serverPort);
            clientSocket = new Socket(serverIP, serverPort);

            in = new DataInputStream(clientSocket.getInputStream());
            out = new PrintStream(clientSocket.getOutputStream());

            System.out.println(in.readLine());

            int estado = 0;
            String enviar, receber;

            while (true) {
                if (estado == 0) {
                    System.out.print("Digite o usuário ('BYE' para sair): ");
                    enviar = teclado.readLine();
                    out.println(enviar);

                    // Gson gson = new Gson();

                    if (enviar.equalsIgnoreCase("BYE")) {
                        break;
                    }

                    receber = in.readLine();
                    System.out.println("Servidor retornou: " + receber);

                    if (receber.equals("USUARIO_OK")) {
                        System.out.println("Usuário aceito. Agora digite a senha.");
                        estado = 1;
                    } else {
                        System.out.println("Usuário não encontrado. Tente novamente.");
                    }
                }

                if (estado == 1) {
                    System.out.print("Digite a senha ('BYE' para sair): ");
                    enviar = teclado.readLine();
                    out.println(enviar);

                    if (enviar.equalsIgnoreCase("BYE")) {
                        break;
                    }

                    receber = in.readLine();

                    if (receber.equals("LOGIN_OK")) {
                        System.out.println("Login realizado com sucesso!");
                        break;
                    } else {
                        System.out.println("Senha incorreta. Tente novamente.");
                    }
                }
            }

        } catch (UnknownHostException e) {
            System.err.println("Host desconhecido: " + e.getMessage());
        } catch (IOException e) {
            System.err.println("Erro de conexão. Verifique o IP/Porta e se o servidor está no ar: " + e.getMessage());
        } catch (NumberFormatException e) {
            System.err.println("Porta inválida. Por favor, digite um número.");
        } finally {
            try {
                if (clientSocket != null) clientSocket.close();
                System.out.println("Conexão encerrada.");
            } catch (IOException e) {
                System.err.println("Erro ao fechar o socket: " + e.getMessage());
            }
        }
    }
}