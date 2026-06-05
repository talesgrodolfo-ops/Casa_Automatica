package luzes.coresystemstgr.chuva;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface ChuvaRepository extends JpaRepository<Chuva, Long> {
}
