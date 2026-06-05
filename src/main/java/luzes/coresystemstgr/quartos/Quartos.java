package luzes.coresystemstgr.quartos;

import com.fasterxml.jackson.annotation.JsonProperty;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.persistence.*;

import com.fasterxml.jackson.annotation.JsonPropertyOrder;

@JsonPropertyOrder({"id", "room", "light", "status"})
@Entity
@Table(name = "quartos")
public class Quartos {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @JsonProperty(access = JsonProperty.Access.READ_ONLY)
    private Long id;

    @Column(name = "room", nullable = false)
    private String room;

    @Column(name = "light")
    @Schema(accessMode = Schema.AccessMode.READ_ONLY)
    private String light = "on";

    @Column(name = "status")
    @Schema(accessMode = Schema.AccessMode.READ_ONLY)
    private String status = "iluminado";

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getRoom() {
        return room;
    }

    public void setRoom(String room) {
        this.room = room;
    }

    public String getLight() {
        return light;
    }

    public void setLight(String light) {
        this.light = light;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }
}