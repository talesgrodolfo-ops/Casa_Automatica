package luzes.coresystemstgr.portao;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.persistence.*;

@JsonPropertyOrder({"id", "gate", "state", "status"})
@Entity
@Table(name = "portao")
public class Portao {

    public static final long PORTAO_ID = 1L;

    @Id
    private Long id = PORTAO_ID;

    @Column(name = "gate_name", nullable = false)
    @JsonProperty("gate")
    private String gateName = "entrada";

    @Column(name = "state")
    @Schema(accessMode = Schema.AccessMode.READ_ONLY)
    private String state = "closed";

    @Column(name = "status")
    @Schema(accessMode = Schema.AccessMode.READ_ONLY)
    private String status = "fechado";

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getGateName() {
        return gateName;
    }

    public void setGateName(String gateName) {
        this.gateName = gateName;
    }

    public String getState() {
        return state;
    }

    public void setState(String state) {
        this.state = state;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }
}
