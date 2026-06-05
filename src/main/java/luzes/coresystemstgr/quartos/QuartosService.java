package luzes.coresystemstgr.quartos;

import org.springframework.stereotype.Service;
import java.util.List;

@Service
public class QuartosService {

    private final QuartosRepository repository;

    public QuartosService(QuartosRepository repository) {
        this.repository = repository;
    }

    public List<Quartos> listarTodos() {
        return repository.findAll();
    }

    public Quartos buscarPorId(Long id) {
        return repository.findById(id)
            .orElseThrow(() -> new RuntimeException("Quarto não encontrado: " + id));
    }

    public Quartos criar(Quartos quarto) {
        return repository.save(quarto);
    }

    public Quartos atualizar(Long id, Quartos dados) {
        Quartos quarto = buscarPorId(id);
        quarto.setRoom(dados.getRoom());
        quarto.setLight(dados.getLight());
        quarto.setStatus(dados.getStatus());
        return repository.save(quarto);
    }

    public void deletar(Long id) {
        repository.deleteById(id);
    }
    public void toggleLight(Long id) {
        Quartos quarto = buscarPorId(id);
        if ("on".equals(quarto.getLight())) {
            quarto.setLight("off");
            quarto.setStatus("apagado");
        } else {
            quarto.setLight("on");
            quarto.setStatus("iluminado");
        }
        repository.save(quarto);
    }
    public void setAllLight() {
        List<Quartos> quartos = listarTodos();
        for (Quartos quarto : quartos) {
            quarto.setLight("on");
            quarto.setStatus("iluminado");
            repository.save(quarto);
        }
    }
    public void setAllDark() {
        List<Quartos> quartos = listarTodos();
        for (Quartos quarto : quartos) {
            quarto.setLight("off");
            quarto.setStatus("apagado");
            repository.save(quarto);
        }
    }
}