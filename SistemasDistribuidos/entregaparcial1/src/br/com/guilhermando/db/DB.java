package br.com.guilhermando.db;
import java.sql.*;
import java.util.ArrayList; 
import java.util.List; 

public class DB {
    private static final String DATABASE_URL = "jdbc:sqlite:data/usuarios.db";

    // Conecta ao banco de dados (ou cria se não existir) e garante que a tabela de usuários exista.
    public static void initializeDatabase() {
        try (Connection conn = DriverManager.getConnection(DATABASE_URL);
             Statement stmt = conn.createStatement()) {

            String sql = "CREATE TABLE IF NOT EXISTS usuarios (" +
                         " id INTEGER PRIMARY KEY AUTOINCREMENT," +
                         " usuario TEXT NOT NULL UNIQUE," +
                         " senha TEXT NOT NULL" +
                         ");";
            stmt.execute(sql);
            System.out.println("Banco de dados inicializado e tabela 'usuarios' pronta.");

            // Adiciona um usuário de exemplo se a tabela estiver vazia
            if (!userExists("usuario123")) {
                 addUser("usuario123", "senha123");
            }

        } catch (SQLException e) {
            System.err.println("Erro ao inicializar o banco de dados: " + e.getMessage());
        }
    }

    // Adiciona um novo usuário.
    public static void addUser(String usuario, String senha) {
        String sql = "INSERT INTO usuarios(usuario, senha) VALUES(?, ?)";
        try (Connection conn = DriverManager.getConnection(DATABASE_URL);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, usuario);
            pstmt.setString(2, senha); // IMPORTANTE: Em um projeto real, a senha deve ser "hasheada"!
            pstmt.executeUpdate();
            System.out.println("Usuário '" + usuario + "' adicionado com sucesso.");
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
            return rs.next(); // Retorna true se encontrou o usuário, false caso contrário.
        } catch (SQLException e) {
            System.err.println("Erro ao verificar usuário: " + e.getMessage());
            return false;
        }
    }

    public static boolean validateLogin(String usuario, String senha) {
        String sql = "SELECT senha FROM usuarios WHERE usuario = ?";
        try (Connection conn = DriverManager.getConnection(DATABASE_URL);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, usuario);
            ResultSet rs = pstmt.executeQuery();
            if (rs.next()) {
                String senhaArmazenada = rs.getString("senha");
                return senhaArmazenada.equals(senha); // Compara a senha enviada com a do banco.
            }
            return false;
        } catch (SQLException e) {
            System.err.println("Erro ao validar login: " + e.getMessage());
            return false;
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
    public static boolean deleteUser(String usuario){
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