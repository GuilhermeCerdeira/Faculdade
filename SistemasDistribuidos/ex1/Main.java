public class Main {
    public static void main(String[] args) {
        Servidor meuServidor = new Servidor();

        Cliente meuCliente = new Cliente(meuServidor);

        meuCliente.enviarMensagem();
    }
}