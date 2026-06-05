package luzes.coresystemstgr.quartos;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface QuartosRepository extends JpaRepository<Quartos, Long> {
    
}