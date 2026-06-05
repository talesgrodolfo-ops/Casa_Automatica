package luzes.coresystemstgr.chuva;

import com.fasterxml.jackson.annotation.JsonPropertyOrder;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.persistence.*;

@JsonPropertyOrder({"rain", "status"})
@Entity
@Table(name = "chuva")
public class Chuva {

    public static final long SENSOR_ID = 1L;

    @Id
    private Long id = SENSOR_ID;

    @Column(name = "rain")
    @Schema(accessMode = Schema.AccessMode.READ_ONLY)
    private String rain = "no";

    @Column(name = "status")
    @Schema(accessMode = Schema.AccessMode.READ_ONLY)
    private String status = "sem chuva";

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getRain() {
        return rain;
    }

    public void setRain(String rain) {
        this.rain = rain;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }
}
