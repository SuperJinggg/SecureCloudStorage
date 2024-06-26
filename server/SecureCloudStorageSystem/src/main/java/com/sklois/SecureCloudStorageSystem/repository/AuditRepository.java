package com.sklois.SecureCloudStorageSystem.repository;

import com.sklois.SecureCloudStorageSystem.entity.AuditEntity;
import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import org.springframework.data.domain.Example;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import java.time.LocalDateTime;

public interface AuditRepository extends JpaRepository<AuditEntity, Integer> {
    long countByRole(String role);

//    @Query(value = "select f from #{#entityName} f where f.time >= ?1 and f.time <= ?2 and f.name=?3 and f.module=?4")
//    Page<AuditEntity> findAll( LocalDateTime starttime, LocalDateTime endtime,String username,String operatemodal, Example<AuditEntity> ex, Pageable pageable);
//    Page<AuditEntity> findAllByTimeGreaterThanEqualAndTimeIsLessThanEqual
    Page<AuditEntity> findAllByTimeAfterAndTimeBeforeAndRoleEquals(LocalDateTime time, LocalDateTime time2,String role,Pageable pageable);
    Page<AuditEntity> findAllByTimeAfterAndTimeBeforeAndModuleEqualsAndRoleEquals(LocalDateTime time, LocalDateTime time2,String operatemodal,String role,Pageable pageable);
    Page<AuditEntity> findAllByTimeAfterAndTimeBeforeAndUsernameEqualsAndRoleEquals(LocalDateTime time, LocalDateTime time2,String username,String role,Pageable pageable);
    Page<AuditEntity> findAllByTimeAfterAndTimeBeforeAndUsernameEqualsAndModuleEqualsAndRoleEquals(LocalDateTime time, LocalDateTime time2, String username, String module, String role,Pageable pageable);
//    @Query(value = "select count(*) from #{#entityName} f where f.time >= ?1 and f.time <= ?2 and f.name=?3 and f.module=?4")

    long countByTimeAfterAndTimeBeforeAndRoleEquals(LocalDateTime time, LocalDateTime time2,String role);
    long countByTimeAfterAndTimeBeforeAndModuleEqualsAndRoleEquals(LocalDateTime time, LocalDateTime time2,String operatemodal,String role);
    long countByTimeAfterAndTimeBeforeAndUsernameEqualsAndRoleEquals(LocalDateTime time, LocalDateTime time2,String username,String role);
    long countByTimeAfterAndTimeBeforeAndUsernameEqualsAndModuleEqualsAndRoleEquals(LocalDateTime time, LocalDateTime time2,String username,String operatemodal, String role);


}
