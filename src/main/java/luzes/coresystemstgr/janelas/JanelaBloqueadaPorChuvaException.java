package luzes.coresystemstgr.janelas;

public class JanelaBloqueadaPorChuvaException extends RuntimeException {

    public JanelaBloqueadaPorChuvaException() {
        super("Não é possível abrir janelas enquanto estiver chovendo.");
    }
}
