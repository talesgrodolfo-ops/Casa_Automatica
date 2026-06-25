package luzes.coresystemstgr.config;

import luzes.coresystemstgr.chuva.ChuvaService;
import luzes.coresystemstgr.janelas.Janela;
import luzes.coresystemstgr.janelas.JanelaRepository;
import luzes.coresystemstgr.portao.PortaoService;
import luzes.coresystemstgr.quartos.Quartos;
import luzes.coresystemstgr.quartos.QuartosRepository;
import org.springframework.boot.ApplicationArguments;
import org.springframework.boot.ApplicationRunner;
import org.springframework.stereotype.Component;

import java.util.List;

@Component
public class DataSeeder implements ApplicationRunner {

    private static final List<String> AMBIENTES_PADRAO = List.of("sala", "quarto", "cozinha", "sótão");
    private static final List<String> JANELAS_PADRAO = List.of("sala", "garagem");

    private final QuartosRepository quartosRepository;
    private final JanelaRepository janelaRepository;
    private final ChuvaService chuvaService;
    private final PortaoService portaoService;

    public DataSeeder(
        QuartosRepository quartosRepository,
        JanelaRepository janelaRepository,
        ChuvaService chuvaService,
        PortaoService portaoService
    ) {
        this.quartosRepository = quartosRepository;
        this.janelaRepository = janelaRepository;
        this.chuvaService = chuvaService;
        this.portaoService = portaoService;
    }

    @Override
    public void run(ApplicationArguments args) {
        chuvaService.obterEstado();
        portaoService.obter();

        if (quartosRepository.count() == 0) {
            for (String ambiente : AMBIENTES_PADRAO) {
                Quartos quarto = new Quartos();
                quarto.setRoom(ambiente);
                quarto.setLight("off");
                quarto.setStatus("apagado");
                quartosRepository.save(quarto);
            }
        }

        if (janelaRepository.count() == 0) {
            for (String ambiente : JANELAS_PADRAO) {
                Janela janela = new Janela();
                janela.setWindowName(ambiente);
                janela.setState("closed");
                janela.setStatus("fechado");
                janelaRepository.save(janela);
            }
        }
    }
}
