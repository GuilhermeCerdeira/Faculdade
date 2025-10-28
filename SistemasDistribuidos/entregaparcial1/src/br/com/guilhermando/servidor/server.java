// src/br/com/guilhermando/servidor/server.java (Versão 4 - A Correta)
package br.com.guilhermando.servidor;

import java.io.*;
import java.net.*;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.io.PrintWriter; // Importa PrintWriter

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.auth0.jwt.interfaces.DecodedJWT;

import br.com.guilhermando.db.DB;
import br.com.guilhermando.protocolo.ErroProtocolo;

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
            // Vou até atualizar a mensagem de log para V4 para não nos confundirmos
            System.out.println("Servidor JSON/JWT (v4) carregado na porta " + porta + ", aguardando conexões...");

        } catch (Exception e) {
            System.err.println("Erro ao iniciar servidor: " + e.getMessage());
            return;
        }

        while (true) {
            try {
                clientSocket = echoServer.accept();
                System.out.println("Cliente conectado: " + clientSocket.getInetAddress().getHostAddress());

                try (
                    BufferedReader is = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                    // Usa PrintWriter com autoFlush=true
                    PrintWriter os = new PrintWriter(clientSocket.getOutputStream(), true)
                ) {
                    os.println("Servidor responde: Conexao efetuada.");

                    String jsonRequest;
                    
                    while ((jsonRequest = is.readLine()) != null) {
                        
                        // Espera Map<String, Object> para aceitar JSON aninhado
                        Map<String, Object> request = gson.fromJson(jsonRequest, new TypeToken<Map<String, Object>>(){}.getType());
                        
                        // Chave "operacao" (sem acento)
                        String operacao = (String) request.get("operacao");
                        
                        System.out.println("Requisição recebida: " + operacao);

                        if (operacao == null) {
                            sendError(os, ErroProtocolo.OPERACAO_INVALIDA);
                            continue;
                        }

                        if ("LOGIN".equals(operacao)) {
                            String user = (String) request.get("usuario");
                            String pass = (String) request.get("senha");
                            if (user == null || pass == null) {
                                sendError(os, ErroProtocolo.CHAVES_FALTANTES);
                                continue;
                            }
                            
                            Map<String, String> userData = DB.validateLogin(user, pass);
                            
                            if (userData != null) {
                                String token = JwtManager.createToken(userData);
                                sendSuccessLogin(os, token);
                            } else {
                                sendError(os, ErroProtocolo.SEM_PERMISSAO);
                            }
                            continue;
                        }

                        if ("CRIAR_USUARIO".equals(operacao)) {
                            Map<String, String> dadosUsuario = null;
                            try {
                                dadosUsuario = (Map<String, String>) request.get("usuario");
                            } catch (Exception e) {
                                sendError(os, ErroProtocolo.CHAVES_FALTANTES);
                                continue;
                            }
                            if (dadosUsuario == null) {
                                sendError(os, ErroProtocolo.CHAVES_FALTANTES);
                                continue;
                            }
                            String newUser = dadosUsuario.get("nome");
                            String newPass = dadosUsuario.get("senha");
                            if (newUser == null || newPass == null) {
                                sendError(os, ErroProtocolo.CHAVES_FALTANTES);
                                continue;
                            }
                            if (newUser.length() < 3 || newUser.length() > 20 || newPass.length() > 20 || newPass.length() < 3) {
                                sendError(os, ErroProtocolo.CAMPOS_INVALIDOS);
                                continue;
                            }
                            if (DB.userExists(newUser)) {
                                sendError(os, ErroProtocolo.USUARIO_JA_EXISTE);
                            } else {
                                DB.addUser(newUser, newPass);
                                sendSuccessCreated(os);
                            }
                            continue;
                        }

                        String token = (String) request.get("token");
                        if (token == null) {
                            sendError(os, ErroProtocolo.CHAVES_FALTANTES);
                            continue;
                        }
                        
                        DecodedJWT claims = JwtManager.validateTokenAndGetClaims(token);
                        if (claims == null) {
                            sendError(os, ErroProtocolo.TOKEN_INVALIDO);
                            continue;
                        }
                        
                        String usuarioDoToken = claims.getClaim("usuario").asString();
                        
                        switch (operacao) {
                            case "LOGOUT":
                                sendSuccess(os);
                                break;
                            
                             case "LISTAR_PROPRIO_USUARIO":
                                Map<String, Object> myInfo = Map.of(
                                    "id", claims.getClaim("id").asString(),
                                    "usuario", usuarioDoToken,
                                    "funcao", claims.getClaim("funcao").asString()
                                );
                                sendSuccess(os, myInfo); 
                                break;
                            
                            case "LISTAR_USUARIOS":
                                List<String> userList = DB.getAllUsers();
                                String userListString = String.join(",", userList);
                                sendSuccess(os, userListString);
                                break;

                            case "EDITAR_PROPRIO_USUARIO":
                                String newPassword = (String) request.get("novasenha");
                                if (newPassword == null) {
                                    sendError(os, ErroProtocolo.CHAVES_FALTANTES);
                                    break;
                                }
                                if(DB.updatePassword(usuarioDoToken, newPassword)){
                                    sendSuccess(os);
                                } else {
                                    sendError(os, ErroProtocolo.FALHA_INTERNA);
                                }
                                break;

                            case "EXCLUIR_PROPRIO_USUARIO":
                                if(DB.deleteUser(usuarioDoToken)){
                                    sendSuccess(os);
                                } else {
                                    sendError(os, ErroProtocolo.FALHA_INTERNA);
                                }
                                break;

                            default:
                                sendError(os, ErroProtocolo.OPERACAO_INVALIDA);
                                break;
                        }
                    }
                }
            } catch (Exception e) {
                System.err.println("Erro grave no servidor: " + e.getMessage());
                e.printStackTrace(); // Adiciona um stack trace para depuração
                if (clientSocket != null && !clientSocket.isClosed()) {
                    try {
                        sendError(new PrintWriter(clientSocket.getOutputStream(), true), ErroProtocolo.FALHA_INTERNA);
                    } catch (IOException io) { /* ignora */ }
                }
            } finally {
                if (clientSocket != null) {
                    try { clientSocket.close(); } catch (IOException e) {}
                    System.out.println("Conexão com o cliente fechada.");
                }
            }
        }
    }

    private static void sendError(PrintWriter os, ErroProtocolo erro) {
        Map<String, Object> response = new HashMap<>();
        response.put("status", erro.getStatusCode());
        response.put("mensagem", erro.getMensagem());
        os.println(gson.toJson(response));
    }

    private static void sendSuccess(PrintWriter os, Object dados) {
        Map<String, Object> response = new HashMap<>();
        response.put("status", "200");
        response.put("mensagem", "Sucesso: Operação realizada com sucesso");
        response.put("dados", dados);
        os.println(gson.toJson(response));
    }

    private static void sendSuccess(PrintWriter os) {
        Map<String, Object> response = new HashMap<>();
        response.put("status", "200");
        response.put("mensagem", "Sucesso: Operação realizada com sucesso");
        os.println(gson.toJson(response));
    }

    private static void sendSuccessLogin(PrintWriter os, String token) {
        Map<String, Object> response = new HashMap<>();
        response.put("status", "200");
        response.put("mensagem", "Sucesso: operação realizada com sucesso");
        response.put("token", token);
        os.println(gson.toJson(response));
    }
    
    private static void sendSuccessCreated(PrintWriter os) {
        Map<String, Object> response = new HashMap<>();
        response.put("status", "201");
        response.put("mensagem", "Sucesso: Recurso cadastrado");
        os.println(gson.toJson(response));
    }
}