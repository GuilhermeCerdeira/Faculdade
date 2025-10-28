package br.com.guilhermando.cliente;

import java.io.*;
import java.net.*;
import java.util.Map;
import java.util.HashMap;
import java.util.List;

import java.io.PrintWriter;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

public class client {
    private static final Gson gson = new Gson();
    private static String currentToken = null;

    public static void main(String[] args) {
        Socket clientSocket = null;
        BufferedReader in = null;
        PrintWriter out = null;
        BufferedReader teclado = new BufferedReader(new InputStreamReader(System.in));

        try {
            System.out.print("Qual o IP do servidor? ");
            String serverIP = teclado.readLine();
            System.out.print("Qual a Porta do servidor? ");
            int serverPort = Integer.parseInt(teclado.readLine());

            System.out.println("Tentando conectar com host " + serverIP + " na porta " + serverPort);
            clientSocket = new Socket(serverIP, serverPort);

            in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            out = new PrintWriter(clientSocket.getOutputStream(), true);

            System.out.println("Servidor: " + in.readLine());

            boolean keepRunning = true;
            while (keepRunning) {
                if (currentToken == null) {
                    System.out.println("\n--- MENU PRINCIPAL ---");
                    System.out.println("1. Fazer Login");
                    System.out.println("2. Cadastrar Novo Usuário");
                    System.out.println("3. Sair");
                    System.out.print("Escolha uma opção: ");
                    String escolha = teclado.readLine();

                    if ("1".equals(escolha)) {
                        realizarLogin(in, out, teclado);
                    } else if ("2".equals(escolha)) {
                        cadastrarUsuario(in, out, teclado);
                    } else if ("3".equals(escolha)) {
                        System.out.println("Saindo...");
                        keepRunning = false;
                    } else {
                        System.out.println("Opção inválida.");
                    }
                } else {
                    keepRunning = menuUsuarioLogado(in, out, teclado);
                }
            }
        } catch (Exception e) {
            System.err.println("Erro: " + e.getMessage());
        } finally {
            try { 
                if (clientSocket != null) clientSocket.close(); 
            } catch (IOException e) {
                System.err.println("Erro ao fechar o socket: " + e.getMessage());
            }
            System.out.println("Conexão encerrada.");
        }
    }

    private static void realizarLogin(BufferedReader in, PrintWriter out, BufferedReader teclado) throws IOException {
        System.out.print("Digite o usuário: ");
        String usuario = teclado.readLine();
        System.out.print("Digite a senha: ");
        String senha = teclado.readLine();

        Map<String, String> request = new HashMap<>();
        request.put("operacao", "LOGIN");
        request.put("usuario", usuario);
        request.put("senha", senha);
        out.println(gson.toJson(request));

        Map<String, Object> response = gson.fromJson(in.readLine(), new TypeToken<Map<String, Object>>(){}.getType());
        System.out.println("Servidor: [" + response.get("status") + "] " + response.get("mensagem"));

        if ("200".equals(response.get("status"))) {
            currentToken = (String) response.get("token");
            System.out.println("Token recebido e armazenado!");
        }
    }

    private static void cadastrarUsuario(BufferedReader in, PrintWriter out, BufferedReader teclado) throws IOException {
        System.out.print("Digite o novo usuário (min. 3 caracteres): ");
        String usuario = teclado.readLine();
        System.out.print("Digite a nova senha (min. 3 caracteres): ");
        String senha = teclado.readLine();

        Map<String, String> dadosUsuario = new HashMap<>();
        dadosUsuario.put("nome", usuario);
        dadosUsuario.put("senha", senha);

        Map<String, Object> request = new HashMap<>();
        request.put("operacao", "CRIAR_USUARIO");
        request.put("usuario", dadosUsuario);
        
        out.println(gson.toJson(request));

        Map<String, Object> response = gson.fromJson(in.readLine(), new TypeToken<Map<String, Object>>(){}.getType());
        System.out.println("Servidor: [" + response.get("status") + "] " + response.get("mensagem"));
    }

    private static boolean menuUsuarioLogado(BufferedReader in, PrintWriter out, BufferedReader teclado) throws IOException {
        System.out.println("\n--- MENU DO USUÁRIO ---");
        System.out.println("1. Ver meus dados");
        System.out.println("2. Listar todos os usuários");
        System.out.println("3. Alterar minha senha");
        System.out.println("4. Deletar minha conta");
        System.out.println("5. Sair (Logout)");
        System.out.print("Escolha uma opção: ");
        String escolha = teclado.readLine();
        
        Map<String, String> request = new HashMap<>();
        request.put("token", currentToken);

        switch (escolha) {
            case "1":
                request.put("operacao", "LISTAR_PROPRIO_USUARIO");
                out.println(gson.toJson(request));
                tratarResposta(in, "LISTAR_PROPRIO_USUARIO");
                break;
            case "2":
                request.put("operacao", "LISTAR_USUARIOS");
                out.println(gson.toJson(request));
                tratarResposta(in, "LISTA_USUARIOS");
                break;
            case "3":
                System.out.print("Digite sua NOVA senha: ");
                String newPassword = teclado.readLine();
                request.put("operacao", "EDITAR_PROPRIO_USUARIO");
                request.put("novasenha", newPassword);
                out.println(gson.toJson(request));
                tratarResposta(in, null);
                break;
            case "4":
                System.out.print("Tem certeza que deseja deletar sua conta? (s/n): ");
                if ("s".equalsIgnoreCase(teclado.readLine())) {
                    request.put("operacao", "EXCLUIR_PROPRIO_USUARIO");
                    out.println(gson.toJson(request));
                    if (tratarResposta(in, null)) {
                        currentToken = null; 
                        return false; 
                    }
                } else {
                    System.out.println("Operação cancelada.");
                }
                break;
            case "5":
                request.put("operacao", "LOGOUT");
                out.println(gson.toJson(request));
                tratarResposta(in, null);
                currentToken = null;
                return false; 
            default:
                System.out.println("Opção inválida.");
                break;
        }
        
        return true;
    }

    private static boolean tratarResposta(BufferedReader in, String tipoAcao) throws IOException {
        Map<String, Object> response = gson.fromJson(in.readLine(), new TypeToken<Map<String, Object>>(){}.getType());
        
        String status = (String) response.get("status");
        System.out.println("Servidor: [" + status + "] " + response.get("mensagem"));

        if ("200".equals(status)) {
            if (tipoAcao != null && tipoAcao.equals("LISTA_USUARIOS")) {
                String usersString = (String) response.get("dados");
                String[] users = usersString.split(",");
                System.out.println("-- Usuários Cadastrados --");
                for (String u : users) {
                    System.out.println("- " + u);
                }
            } else if (response.containsKey("dados")) {
                System.out.println("Dados: " + response.get("dados"));
            }
            return true;
        } else {
            if ("401".equals(status)) {
                System.out.println("Token inválido ou expirado. Deslogando...");
                currentToken = null;
            }
            return false;
        }
    }
}