// SERVIDOR 
import java.io.*;
import java.net.*;

public class EchoServerTCP_Server {

    public static void main(String args[]) {
        ServerSocket echoServer = null;
        Socket clientSocket = null;
        int porta;

        String usuarioCorreto = "usuario123";
        String senhaCorreta = "senha123";

        try {
            System.out.println("Qual porta o servidor deve usar? ");
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            porta = Integer.parseInt(br.readLine());
            echoServer = new ServerSocket(porta);
            System.out.println("Servidor carregado na porta " + porta + ", aguardando conexões...");

        } catch (IOException e) {
            System.err.println("Não foi possível iniciar o servidor na porta especificada: " + e.getMessage());
            return; 
        }

        while (true) {
            try {
                clientSocket = echoServer.accept();
                System.out.println("Cliente conectado: " + clientSocket.getInetAddress().getHostAddress());

                try (
                    DataInputStream is = new DataInputStream(clientSocket.getInputStream());
                    PrintStream os = new PrintStream(clientSocket.getOutputStream())
                ) {
                    os.println("Servidor responde: Conexao efetuada. Por favor, identifique-se.");

                    String line;
                    boolean usuarioValido = false;

                    while ((line = is.readLine()) != null) {
                        System.out.println("Cliente enviou: " + line);

                        if (line.equalsIgnoreCase("BYE")) {
                            System.out.println("Cliente desconectou.");
                            break;
                        }

                        if (!usuarioValido) {
                            if (line.equals(usuarioCorreto)) {
                                usuarioValido = true;
                                os.println("USUARIO_OK");
                                System.out.println("Servidor respondeu: USUARIO_OK");
                            } else {
                                os.println("USUARIO_INVALIDO");
                                System.out.println("Servidor respondeu: USUARIO_INVALIDO");
                            }
                        }
                        else {
                            if (line.equals(senhaCorreta)) {
                                os.println("LOGIN_OK");
                                System.out.println("Servidor respondeu: LOGIN_OK");
                                usuarioValido = false;
                            } else {
                                os.println("SENHA_INVALIDA");
                                System.out.println("Servidor respondeu: SENHA_INVALIDA");
                            }
                        }
                    }
                }

            } catch (IOException e) {
                System.err.println("Erro na comunicação com o cliente: " + e.getMessage());
            } finally {
                System.out.println("Fechando conexão com o cliente.");
            }
        }
    }
}