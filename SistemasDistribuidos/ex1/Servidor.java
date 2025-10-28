public class Servidor {
    public String processarMensagem(String mensagem) {
        System.out.println("[Servidor] Mensagem recebida: " + mensagem);
        String resposta = mensagem.toUpperCase();
        System.out.println("[Servidor] Enviando resposta: " + resposta);
        return resposta;
    }
}