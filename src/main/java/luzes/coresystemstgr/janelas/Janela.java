package luzes.coresystemstgr.janelas;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.persistence.*;

@JsonPropertyOrder({"id", "window", "state", "status"})
@Entity
@Table(name = "janelas")
public class Janela {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @JsonProperty(access = JsonProperty.Access.READ_ONLY)
    private Long id;

    @Column(name = "window_name", nullable = false)
    @JsonProperty("window")
    private String windowName;

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

    public String getWindowName() {
        return windowName;
    }

    public void setWindowName(String windowName) {
        this.windowName = windowName;
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
