package br.com.guilhermando.db;

import java.sql.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List; // Importar Map
import java.util.Map; // Importar HashMap

public class DB {
    private static final String DATABASE_URL = "jdbc:sqlite:data/usuarios.db";

    public static void initializeDatabase() {
        try (Connection conn = DriverManager.getConnection(DATABASE_URL);
             Statement stmt = conn.createStatement()) {

            // ---> MUDANÇA AQUI: Adicionada a coluna 'funcao' <---
            String sql = "CREATE TABLE IF NOT EXISTS usuarios (" +
                         " id INTEGER PRIMARY KEY AUTOINCREMENT," +
                         " usuario TEXT NOT NULL UNIQUE," +
                         " senha TEXT NOT NULL," +
                         " funcao TEXT NOT NULL DEFAULT 'user'" + 
                         ");";
            stmt.execute(sql);
            System.out.println("Banco de dados inicializado e tabela 'usuarios' pronta.");

            if (!userExists("admin")) {
                 // Adiciona um usuário admin para teste
                 addUser("admin", "admin", "admin");
                 addUser("usuario", "usuario", "user");
            }

        } catch (SQLException e) {
            System.err.println("Erro ao inicializar o banco de dados: " + e.getMessage());
        }
    }

    public static void addUser(String usuario, String senha) {
        addUser(usuario, senha, "user");
    }

    public static void addUser(String usuario, String senha, String funcao) {
        String sql = "INSERT INTO usuarios(usuario, senha, funcao) VALUES(?, ?, ?)";
        try (Connection conn = DriverManager.getConnection(DATABASE_URL);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, usuario);
            pstmt.setString(2, senha);
            pstmt.setString(3, funcao);
            pstmt.executeUpdate();
            System.out.println("Usuário '" + usuario + "' ("+ funcao +") adicionado com sucesso.");
        } catch (SQLException e) {
            System.err.println("Erro ao adicionar usuário: " + e.getMessage());
        }
    }

    public static boolean userExists(String usuario) {
        String sql = "SELECT id FROM usuarios WHERE usuario = ?";
        try (Connection conn = DriverManager.getConnection(DATABASE_URL);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, usuario);
            ResultSet rs = pstmt.executeQuery();
            return rs.next();
        } catch (SQLException e) {
            System.err.println("Erro ao verificar usuário: " + e.getMessage());
            return false;
        }
    }

    public static Map<String, String> validateLogin(String usuario, String senha) {
        String sql = "SELECT id, usuario, funcao FROM usuarios WHERE usuario = ? AND senha = ?";
        try (Connection conn = DriverManager.getConnection(DATABASE_URL);
            PreparedStatement pstmt = conn.prepareStatement(sql)) {
            
            pstmt.setString(1, usuario);
            pstmt.setString(2, senha);
            ResultSet rs = pstmt.executeQuery();
            
            if (rs.next()) {
                Map<String, String> userData = new HashMap<>();
                userData.put("id", String.valueOf(rs.getInt("id"))); 
                userData.put("usuario", rs.getString("usuario"));
                userData.put("funcao", rs.getString("funcao"));
                return userData;
            }
            return null;
        } catch (SQLException e) {
            System.err.println("Erro ao validar login: " + e.getMessage());
            return null;
        }
    }

    
    public static List<String> getAllUsers() {
        List<String> users = new ArrayList<>();
        String sql = "SELECT usuario FROM usuarios ORDER BY usuario ASC";
        try (Connection conn = DriverManager.getConnection(DATABASE_URL);
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                users.add(rs.getString("usuario"));
            }
        } catch (SQLException e) {
            System.err.println("Erro ao buscar usuários: " + e.getMessage());
        }
        return users;
    }

    public static boolean deleteUser(String usuario) {
        String sql = "DELETE FROM usuarios WHERE usuario = ?";
        try (Connection conn = DriverManager.getConnection(DATABASE_URL);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, usuario);
            int affectedRows = pstmt.executeUpdate();
            return affectedRows > 0;
        } catch (SQLException e) {
            System.err.println("Erro ao deletar usuário " + usuario + ": " + e.getMessage());
            return false;
        }
    }

    public static boolean updatePassword(String usuario, String novaSenha){
        String sql = "UPDATE usuarios SET senha = ? WHERE usuario = ?";
        try (Connection conn = DriverManager.getConnection(DATABASE_URL);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, novaSenha);
            pstmt.setString(2, usuario);
            int affectedRows = pstmt.executeUpdate();
            return affectedRows > 0;
        }catch (SQLException e){
            System.err.println("Erro ao atualizar a senha do usuario " + usuario + ": " + e.getMessage());
            return false;
        }
    }
}