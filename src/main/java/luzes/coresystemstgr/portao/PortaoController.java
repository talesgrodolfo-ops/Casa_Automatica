package luzes.coresystemstgr.portao;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api/portao")
@Tag(name = "Portão", description = "Controle do portão de entrada")
public class PortaoController {

    private final PortaoService service;

    public PortaoController(PortaoService service) {
        this.service = service;
    }

    @GetMapping
    @Operation(summary = "Retorna o estado do portão")
    public ResponseEntity<Portao> obter() {
        return ResponseEntity.ok(service.obter());
    }

    @PostMapping("/toggle")
    @Operation(summary = "Alterna aberto/fechado do portão")
    public ResponseEntity<Portao> toggle() {
        return ResponseEntity.ok(service.toggle());
    }
}
