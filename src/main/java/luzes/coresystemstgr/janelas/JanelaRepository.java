package luzes.coresystemstgr.janelas;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface JanelaRepository extends JpaRepository<Janela, Long> {
}
