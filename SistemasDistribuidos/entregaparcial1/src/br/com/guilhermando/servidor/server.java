package br.com.guilhermando.servidor;

import br.com.guilhermando.db.DB; 

import java.io.*;
import java.net.*;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

public class server {
    private static final Gson gson = new Gson();

    public static void main(String args[]) {
        ServerSocket echoServer = null;
        Socket clientSocket = null;
        int porta;

        DB.initializeDatabase();

        try {
            System.out.print("Qual porta o servidor deve usar? ");
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            porta = Integer.parseInt(br.readLine());
            echoServer = new ServerSocket(porta);
            System.out.println("Servidor JSON carregado na porta " + porta + ", aguardando conexões...");

        } catch (IOException e) {
            System.err.println("Não foi possível iniciar o servidor na porta especificada: " + e.getMessage());
            return;
        } catch (NumberFormatException e) {
            System.err.println("Porta inválida. Por favor, digite um número.");
            return;
        }


        while (true) {
            try {
                clientSocket = echoServer.accept();
                System.out.println("Cliente conectado: " + clientSocket.getInetAddress().getHostAddress());

                try (
                    BufferedReader is = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                    PrintStream os = new PrintStream(clientSocket.getOutputStream())
                ) {
                    os.println("Servidor responde: Conexao efetuada.");

                    String jsonRequest;
                    String usuarioLogado = null;

                    while ((jsonRequest = is.readLine()) != null) {
                        Map<String, String> request = gson.fromJson(jsonRequest, new TypeToken<Map<String, String>>(){}.getType());
                        String operacao = request.get("operacao");
                        
                        System.out.println("Cliente (" + (usuarioLogado != null ? usuarioLogado : "não logado") + ") pediu: " + operacao);

                        if ("LOGOUT".equals(operacao)) {
                            System.out.println("Usuário " + usuarioLogado + " fez logout.");
                            usuarioLogado = null;
                            sendResponse(os, "sucesso", "Logout bem-sucedido", null);
                            continue;
                        }
                        
                        switch (operacao) {
                            case "LOGIN":
                                String user = request.get("usuario");
                                String pass = request.get("senha");
                                if (DB.validateLogin(user, pass)) {
                                    usuarioLogado = user;
                                    sendResponse(os, "sucesso", "Login realizado com sucesso!", null);
                                } else {
                                    sendResponse(os, "erro", "Usuário ou senha inválidos", null);
                                }
                                break;
                            
                            case "REGISTER":
                                String newUser = request.get("usuario");
                                String newPass = request.get("senha");
                                if (DB.userExists(newUser)) {
                                    sendResponse(os, "erro", "Usuário já existe", null);
                                } else {
                                    DB.addUser(newUser, newPass);
                                    sendResponse(os, "sucesso", "Usuário cadastrado com sucesso", null);
                                }
                                break;
                            
                            case "GET_MY_INFO":
                                if (usuarioLogado != null) {
                                    sendResponse(os, "sucesso", null, Map.of("usuario", usuarioLogado));
                                } else {
                                    sendResponse(os, "erro", "Acesso negado. Faça login primeiro.", null);
                                }
                                break;
                            
                            case "LIST_USERS":
                                if (usuarioLogado != null) {
                                    List<String> userList = DB.getAllUsers();
                                    sendResponse(os, "sucesso", null, userList);
                                } else {
                                    sendResponse(os, "erro", "Acesso negado. Faça login primeiro.", null);
                                }
                                break;
                            
                            case "UPDATE_PASSWORD":
                                if (usuarioLogado != null) {
                                    String newPassword = request.get("nova_senha");
                                    if(DB.updatePassword(usuarioLogado, newPassword)){
                                        sendResponse(os, "sucesso", "Senha alterada com sucesso", null);
                                    } else {
                                        sendResponse(os, "erro", "Falha ao atualizar a senha", null);
                                    }
                                } else {
                                     sendResponse(os, "erro", "Acesso negado. Faça login primeiro.", null);
                                }
                                break;

                            case "DELETE_ACCOUNT":
                                if (usuarioLogado != null) {
                                    if(DB.deleteUser(usuarioLogado)){
                                        usuarioLogado = null; // Desloga após deletar
                                        sendResponse(os, "sucesso", "Conta deletada com sucesso", null);
                                    } else {
                                        sendResponse(os, "erro", "Falha ao deletar a conta", null);
                                    }
                                } else {
                                     sendResponse(os, "erro", "Acesso negado. Faça login primeiro.", null);
                                }
                                break;

                            default:
                                sendResponse(os, "erro", "Operação desconhecida", null);
                                break;
                        }
                    }
                }
            } catch (Exception e) {
                System.err.println("Erro na comunicação com o cliente: " + e.getMessage());
            } finally {
                if (clientSocket != null) {
                    try { clientSocket.close(); } catch (IOException e) {}
                    System.out.println("Conexão com o cliente fechada.");
                }
            }
        }
    }

    /**
     * Helper para criar e enviar uma resposta JSON padronizada para o cliente.
     * @param os O PrintStream do cliente.
     * @param status "sucesso" ou "erro".
     * @param mensagem Uma mensagem descritiva (pode ser null).
     * @param dados Um objeto (Mapa, Lista, etc.) com os dados da resposta (pode ser null).
     */
    private static void sendResponse(PrintStream os, String status, String mensagem, Object dados) {
        Map<String, Object> response = new HashMap<>();
        response.put("status", status);
        if (mensagem != null) response.put("mensagem", mensagem);
        if (dados != null) response.put("dados", dados);
        
        os.println(gson.toJson(response));
    }
}