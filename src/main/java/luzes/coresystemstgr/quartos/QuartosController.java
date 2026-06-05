package luzes.coresystemstgr.quartos;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import java.util.Map;

@RestController
@RequestMapping("/api/quartos")
@Tag(name = "Quartos", description = "Gerenciamento de quartos")
public class QuartosController {

    private final QuartosService service;

    public QuartosController(QuartosService service) {
        this.service = service;
    }

    @GetMapping
    @Operation(summary = "Lista todos os quartos")
    public ResponseEntity<?> listarTodos() {
        return ResponseEntity.ok(Map.of("rooms", service.listarTodos()));
    }

    @GetMapping("/{id}")
    @Operation(summary = "Busca quarto por ID")
    public ResponseEntity<Quartos> buscarPorId(@PathVariable Long id) {
        return ResponseEntity.ok(service.buscarPorId(id));
    }

    @PostMapping
    @Operation(summary = "Cria um novo quarto")
    public ResponseEntity<Quartos> criar(@RequestBody Quartos quarto) {
        return ResponseEntity.status(201).body(service.criar(quarto));
    }

    @PutMapping("/{id}")
    @Operation(summary = "Atualiza um quarto")
    public ResponseEntity<Quartos> atualizar(@PathVariable Long id, @RequestBody Quartos quarto) {
        return ResponseEntity.ok(service.atualizar(id, quarto));
    }

    @DeleteMapping("/{id}")
    @Operation(summary = "Deleta um quarto")
    public ResponseEntity<Void> deletar(@PathVariable Long id) {
        service.deletar(id);
        return ResponseEntity.noContent().build();
    }
    @PostMapping("/{id}/toggle")
    @Operation(summary = "Alterna o estado da luz do quarto")
    public ResponseEntity<Void> toggleLight(@PathVariable Long id) {
        service.toggleLight(id);
        return ResponseEntity.ok().build();
    }
    @PostMapping("/all/on")
    @Operation(summary = "Liga todas as luzes dos quartos")
    public ResponseEntity<Void> setAllLight() {
        service.setAllLight();
        return ResponseEntity.ok().build();
    }
    @PostMapping("/all/off")
    @Operation(summary = "Desliga todas as luzes dos quartos")
    public ResponseEntity<Void> setAllDark() {
        service.setAllDark();
        return ResponseEntity.ok().build();
    }
}