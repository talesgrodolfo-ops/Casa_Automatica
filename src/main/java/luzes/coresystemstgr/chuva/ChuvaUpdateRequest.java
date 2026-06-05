package luzes.coresystemstgr.chuva;

import io.swagger.v3.oas.annotations.media.Schema;

public class ChuvaUpdateRequest {

    @Schema(description = "Estado do sensor: yes/no (ou on/off)", example = "yes")
    private String rain;

    public String getRain() {
        return rain;
    }

    public void setRain(String rain) {
        this.rain = rain;
    }
}
