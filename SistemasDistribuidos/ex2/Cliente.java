import java.awt.*; 
import javax.swing.*; 

public class Cliente extends JFrame { 

    private JTextField campoMensagem; 
    private final JButton botaoEnviar;      
    private JLabel labelResposta;   
    private Servidor servidor; 

    public Cliente() {
        super("aousdhaoudhaídhaósidáoidoaíd"); 
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 600);
        setLayout(new FlowLayout(FlowLayout.CENTER, 50, 50));
        this.servidor = new Servidor();
        JLabel texto = new JLabel("Digite sua mensagem:");
        texto.setFont(new Font("SansSerif", Font.BOLD, 14));
        add(texto);
        campoMensagem = new JTextField(20);
        add(campoMensagem);
        botaoEnviar = new JButton("Enviar");
        add(botaoEnviar);
        labelResposta = new JLabel("Aguardando resposta do servidor...");
        add(labelResposta);
        botaoEnviar.addActionListener(e -> {
            String mensagemParaServidor = campoMensagem.getText();
            String respostaDoServidor = servidor.processarMensagem(mensagemParaServidor);
            labelResposta.setText("Servidor respondeu: " + respostaDoServidor);
        });
        setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new Cliente());
    }
}