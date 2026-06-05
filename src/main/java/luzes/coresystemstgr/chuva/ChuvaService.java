package luzes.coresystemstgr.chuva;

import org.springframework.stereotype.Service;

@Service
public class ChuvaService {

    private final ChuvaRepository repository;

    public ChuvaService(ChuvaRepository repository) {
        this.repository = repository;
    }

    public Chuva obterEstado() {
        return repository.findById(Chuva.SENSOR_ID).orElseGet(this::criarPadrao);
    }

    public Chuva atualizarDoEsp(String rainInput) {
        Chuva chuva = obterEstado();
        String rain = normalizarRain(rainInput);
        chuva.setRain(rain);
        chuva.setStatus("yes".equals(rain) ? "chovendo" : "sem chuva");
        return repository.save(chuva);
    }

    public Chuva criarPadrao() {
        Chuva chuva = new Chuva();
        chuva.setId(Chuva.SENSOR_ID);
        chuva.setRain("no");
        chuva.setStatus("sem chuva");
        return repository.save(chuva);
    }

    private String normalizarRain(String rainInput) {
        if (rainInput == null || rainInput.isBlank()) {
            throw new IllegalArgumentException("Campo rain é obrigatório (yes/no ou on/off)");
        }
        String value = rainInput.trim().toLowerCase();
        if ("yes".equals(value) || "on".equals(value) || "true".equals(value) || "1".equals(value)
            || "chovendo".equals(value)) {
            return "yes";
        }
        if ("no".equals(value) || "off".equals(value) || "false".equals(value) || "0".equals(value)
            || "sem chuva".equals(value) || "nao".equals(value) || "não".equals(value)) {
            return "no";
        }
        throw new IllegalArgumentException("Valor de rain inválido: " + rainInput);
    }
}
