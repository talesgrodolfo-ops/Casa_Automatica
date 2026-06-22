package luzes.coresystemstgr;

import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.context.TestPropertySource;

@SpringBootTest
@ActiveProfiles("test")
@TestPropertySource(properties = {
	"casa-automatica.security.user.password=test-password-12",
	"casa-automatica.security.device-api-key=test-device-api-key-32-characters-min"
})
class CoresystemstgrApplicationTests {

	@Test
	void contextLoads() {
	}

}
