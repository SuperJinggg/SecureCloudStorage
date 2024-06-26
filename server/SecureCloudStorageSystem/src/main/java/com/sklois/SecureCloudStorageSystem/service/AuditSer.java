package com.sklois.SecureCloudStorageSystem.service;

import com.sklois.SecureCloudStorageSystem.entity.AuditEntity;
import org.springframework.data.domain.Page;

import java.time.LocalDateTime;

public interface AuditSer {
    public Page<AuditEntity> listByPage(int pageNo, int pageSize, AuditEntity entity);
    public long countByRole(String role);

    public  AuditEntity auditLogSave(LocalDateTime time,
                                     String ipaddress,
                                     String username,
                                     String role,
                                     String method,
                                     String module,
                                     String object,
                                     String result,
                                     String status);
    public Page<AuditEntity> auditLogSearch(
            LocalDateTime starttime, LocalDateTime endtime,String username,String operatemodal,
            int pageNo, int pageSize, String  role
    );
    public long countAuditLog(LocalDateTime starttime, LocalDateTime endtime,String username,String operatemodal,String role);
}
