package luzes.coresystemstgr.janelas;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Map;

@RestController
@RequestMapping("/api/janelas")
@Tag(name = "Janelas", description = "Controle de janelas (aberto/fechado)")
public class JanelaController {

    private final JanelaService service;

    public JanelaController(JanelaService service) {
        this.service = service;
    }

    @GetMapping
    @Operation(summary = "Lista todas as janelas")
    public ResponseEntity<?> listarTodos() {
        return ResponseEntity.ok(Map.of(
            "windows", service.listarTodos(),
            "windowsBlocked", service.janelasBloqueadas()
        ));
    }

    @GetMapping("/{id}")
    @Operation(summary = "Busca janela por ID")
    public ResponseEntity<Janela> buscarPorId(@PathVariable Long id) {
        return ResponseEntity.ok(service.buscarPorId(id));
    }

    @PostMapping
    @Operation(summary = "Cadastra uma nova janela")
    public ResponseEntity<Janela> criar(@RequestBody Janela janela) {
        return ResponseEntity.status(201).body(service.criar(janela));
    }

    @PutMapping("/{id}")
    @Operation(summary = "Atualiza uma janela")
    public ResponseEntity<Janela> atualizar(@PathVariable Long id, @RequestBody Janela janela) {
        return ResponseEntity.ok(service.atualizar(id, janela));
    }

    @DeleteMapping("/{id}")
    @Operation(summary = "Remove uma janela")
    public ResponseEntity<Void> deletar(@PathVariable Long id) {
        service.deletar(id);
        return ResponseEntity.noContent().build();
    }

    @PostMapping("/{id}/toggle")
    @Operation(summary = "Alterna aberto/fechado da janela")
    public ResponseEntity<Void> toggle(@PathVariable Long id) {
        service.toggle(id);
        return ResponseEntity.ok().build();
    }

    @PostMapping("/all/open")
    @Operation(summary = "Abre todas as janelas")
    public ResponseEntity<Void> abrirTodas() {
        service.abrirTodas();
        return ResponseEntity.ok().build();
    }

    @PostMapping("/all/closed")
    @Operation(summary = "Fecha todas as janelas")
    public ResponseEntity<Void> fecharTodas() {
        service.fecharTodas();
        return ResponseEntity.ok().build();
    }
}
