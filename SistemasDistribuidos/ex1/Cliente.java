public class Cliente {
    private final Servidor servidor;
    public Cliente(Servidor servidor) {
        this.servidor = servidor;
    }
    public void enviarMensagem() {
        String mensagem = "penis";
        
        System.out.println("[Cliente] Enviando mensagem: " + mensagem);
        String resposta = servidor.processarMensagem(mensagem);
        
        System.out.println("[Cliente] Resposta recebida: " + resposta);
    }
}