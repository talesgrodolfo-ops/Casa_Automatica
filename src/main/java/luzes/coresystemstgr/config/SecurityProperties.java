package luzes.coresystemstgr.config;

import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.validation.annotation.Validated;

@ConfigurationProperties(prefix = "lighthouse.security")
@Validated
public class SecurityProperties {

    @Valid
    private User user = new User();

    @NotBlank(message = "Defina lighthouse.security.device-api-key ou LIGHTHOUSE_DEVICE_API_KEY")
    @Size(min = 32, message = "A chave do ESP32 deve ter pelo menos 32 caracteres")
    private String deviceApiKey;

    @Valid
    private RateLimit rateLimit = new RateLimit();

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public String getDeviceApiKey() {
        return deviceApiKey;
    }

    public void setDeviceApiKey(String deviceApiKey) {
        this.deviceApiKey = deviceApiKey;
    }

    public RateLimit getRateLimit() {
        return rateLimit;
    }

    public void setRateLimit(RateLimit rateLimit) {
        this.rateLimit = rateLimit;
    }

    @Validated
    public static class User {

        @NotBlank(message = "Defina lighthouse.security.user.name ou LIGHTHOUSE_USER")
        private String name = "admin";

        @NotBlank(message = "Defina lighthouse.security.user.password ou LIGHTHOUSE_PASSWORD")
        @Size(min = 12, message = "A senha do usuário deve ter pelo menos 12 caracteres")
        private String password;

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public String getPassword() {
            return password;
        }

        public void setPassword(String password) {
            this.password = password;
        }
    }

    public static class RateLimit {
        private int requestsPerMinute = 120;

        public int getRequestsPerMinute() {
            return requestsPerMinute;
        }

        public void setRequestsPerMinute(int requestsPerMinute) {
            this.requestsPerMinute = requestsPerMinute;
        }
    }
}
