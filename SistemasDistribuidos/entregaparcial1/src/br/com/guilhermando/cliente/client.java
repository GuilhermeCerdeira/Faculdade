// CLIENTE (versão JSON com Gson)
package br.com.guilhermando.cliente;

import java.io.*;
import java.net.*;
import java.util.Map;
import java.util.HashMap;
import java.util.List;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

public class client {
    private static final Gson gson = new Gson(); // Instância única de Gson

    public static void main(String[] args) {
        // ... (código de conexão inicial permanece o mesmo)
        Socket clientSocket = null;
        BufferedReader in = null;
        PrintStream out = null;
        BufferedReader teclado = new BufferedReader(new InputStreamReader(System.in));

        try {
            System.out.print("Qual o IP do servidor? ");
            String serverIP = teclado.readLine();
            System.out.print("Qual a Porta do servidor? ");
            int serverPort = Integer.parseInt(teclado.readLine());

            System.out.println("Tentando conectar com host " + serverIP + " na porta " + serverPort);
            clientSocket = new Socket(serverIP, serverPort);

            in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            out = new PrintStream(clientSocket.getOutputStream());

            System.out.println("Servidor: " + in.readLine());

            // Loop do Menu Principal
            while (true) {
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
                    break;
                } else {
                    System.out.println("Opção inválida.");
                }
            }
        } catch (Exception e) {
            System.err.println("Erro: " + e.getMessage());
        } finally {
            try { if (clientSocket != null) clientSocket.close(); } catch (IOException e) {}
            System.out.println("Conexão encerrada.");
        }
    }

    private static void realizarLogin(BufferedReader in, PrintStream out, BufferedReader teclado) throws IOException {
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
        System.out.println("Servidor: " + response.get("mensagem"));

        if ("sucesso".equals(response.get("status"))) {
            menuUsuarioLogado(in, out, teclado);
        }
    }

    private static void cadastrarUsuario(BufferedReader in, PrintStream out, BufferedReader teclado) throws IOException {
        System.out.print("Digite o novo usuário: ");
        String usuario = teclado.readLine();
        System.out.print("Digite a nova senha: ");
        String senha = teclado.readLine();

        Map<String, String> request = new HashMap<>();
        request.put("operacao", "REGISTER");
        request.put("usuario", usuario);
        request.put("senha", senha);
        out.println(gson.toJson(request));

        Map<String, Object> response = gson.fromJson(in.readLine(), new TypeToken<Map<String, Object>>(){}.getType());
        System.out.println("Servidor: " + response.get("mensagem"));
    }

    private static void menuUsuarioLogado(BufferedReader in, PrintStream out, BufferedReader teclado) throws IOException {
        while (true) {
            System.out.println("\n--- MENU DO USUÁRIO ---");
            System.out.println("1. Ver meus dados");
            System.out.println("2. Listar todos os usuários");
            System.out.println("3. Alterar minha senha");
            System.out.println("4. Deletar minha conta");
            System.out.println("5. Sair (Logout)");
            System.out.print("Escolha uma opção: ");
            String escolha = teclado.readLine();
            
            Map<String, String> request = new HashMap<>();
            boolean fazerLogout = false;

            switch (escolha) {
                case "1":
                    request.put("operacao", "GET_MY_INFO");
                    out.println(gson.toJson(request));
                    Map<String, Object> myInfoResponse = gson.fromJson(in.readLine(), new TypeToken<Map<String, Object>>(){}.getType());
                    if("sucesso".equals(myInfoResponse.get("status"))){
                        System.out.println("Seus dados: " + myInfoResponse.get("dados"));
                    } else {
                        System.out.println("Erro: " + myInfoResponse.get("mensagem"));
                    }
                    break;
                case "2":
                    request.put("operacao", "LIST_USERS");
                    out.println(gson.toJson(request));
                    Map<String, Object> listResponse = gson.fromJson(in.readLine(), new TypeToken<Map<String, Object>>(){}.getType());
                    if("sucesso".equals(listResponse.get("status"))){
                        List<String> users = (List<String>) listResponse.get("dados");
                        System.out.println("-- Usuários Cadastrados --");
                        users.forEach(u -> System.out.println("- " + u));
                    } else {
                        System.out.println("Erro: " + listResponse.get("mensagem"));
                    }
                    break;
                case "3":
                    System.out.print("Digite sua NOVA senha: ");
                    String newPassword = teclado.readLine();
                    request.put("operacao", "UPDATE_PASSWORD");
                    request.put("nova_senha", newPassword);
                    out.println(gson.toJson(request));
                    Map<String, Object> updateResponse = gson.fromJson(in.readLine(), new TypeToken<Map<String, Object>>(){}.getType());
                    System.out.println("Servidor: " + updateResponse.get("mensagem"));
                    break;
                case "4":
                    System.out.print("Tem certeza que deseja deletar sua conta? (s/n): ");
                    if ("s".equalsIgnoreCase(teclado.readLine())) {
                        request.put("operacao", "DELETE_ACCOUNT");
                        out.println(gson.toJson(request));
                        Map<String, Object> deleteResponse = gson.fromJson(in.readLine(), new TypeToken<Map<String, Object>>(){}.getType());
                        System.out.println("Servidor: " + deleteResponse.get("mensagem"));
                        if ("sucesso".equals(deleteResponse.get("status"))) {
                            return; // Sai do menu logado
                        }
                    } else {
                        System.out.println("Operação cancelada.");
                    }
                    break;
                case "5":
                    request.put("operacao", "LOGOUT");
                    out.println(gson.toJson(request));
                    Map<String, Object> logoutResponse = gson.fromJson(in.readLine(), new TypeToken<Map<String, Object>>(){}.getType());
                    System.out.println("Servidor: " + logoutResponse.get("mensagem"));
                    fazerLogout = true;
                    break;
                default:
                    System.out.println("Opção inválida.");
                    break;
            }
            if (fazerLogout) break;
        }
    }
}