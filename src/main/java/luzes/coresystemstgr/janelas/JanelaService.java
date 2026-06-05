package luzes.coresystemstgr.janelas;

import luzes.coresystemstgr.chuva.ChuvaService;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class JanelaService {

    private final JanelaRepository repository;
    private final ChuvaService chuvaService;

    public JanelaService(JanelaRepository repository, ChuvaService chuvaService) {
        this.repository = repository;
        this.chuvaService = chuvaService;
    }

    public boolean janelasBloqueadas() {
        return "yes".equals(chuvaService.obterEstado().getRain());
    }

    public List<Janela> listarTodos() {
        return repository.findAll();
    }

    public Janela buscarPorId(Long id) {
        return repository.findById(id)
            .orElseThrow(() -> new RuntimeException("Janela não encontrada: " + id));
    }

    public Janela criar(Janela janela) {
        aplicarStatus(janela);
        if ("open".equals(janela.getState())) {
            verificarPodeAbrir();
        }
        return repository.save(janela);
    }

    public Janela atualizar(Long id, Janela dados) {
        Janela janela = buscarPorId(id);
        janela.setWindowName(dados.getWindowName());
        if (dados.getState() != null) {
            janela.setState(normalizarState(dados.getState()));
        }
        aplicarStatus(janela);
        if ("open".equals(janela.getState())) {
            verificarPodeAbrir();
        }
        return repository.save(janela);
    }

    public void deletar(Long id) {
        repository.deleteById(id);
    }

    public void toggle(Long id) {
        Janela janela = buscarPorId(id);
        if ("closed".equals(janela.getState())) {
            verificarPodeAbrir();
            janela.setState("open");
            janela.setStatus("aberto");
        } else {
            janela.setState("closed");
            janela.setStatus("fechado");
        }
        repository.save(janela);
    }

    public void abrirTodas() {
        verificarPodeAbrir();
        for (Janela janela : listarTodos()) {
            janela.setState("open");
            janela.setStatus("aberto");
            repository.save(janela);
        }
    }

    public void fecharTodas() {
        for (Janela janela : repository.findAll()) {
            janela.setState("closed");
            janela.setStatus("fechado");
            repository.save(janela);
        }
    }

    private void verificarPodeAbrir() {
        if (janelasBloqueadas()) {
            throw new JanelaBloqueadaPorChuvaException();
        }
    }

    private void aplicarStatus(Janela janela) {
        janela.setState(normalizarState(janela.getState()));
        janela.setStatus("open".equals(janela.getState()) ? "aberto" : "fechado");
    }

    private String normalizarState(String state) {
        if (state == null) {
            return "closed";
        }
        String value = state.trim().toLowerCase();
        if ("open".equals(value) || "aberto".equals(value) || "aberta".equals(value)) {
            return "open";
        }
        return "closed";
    }
}
