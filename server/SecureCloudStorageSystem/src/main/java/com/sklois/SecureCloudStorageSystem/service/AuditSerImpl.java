package com.sklois.SecureCloudStorageSystem.service;

import com.sklois.SecureCloudStorageSystem.entity.AuditEntity;
import com.sklois.SecureCloudStorageSystem.repository.AuditRepository;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.*;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

@Service
public class AuditSerImpl implements AuditSer {
    @Autowired
    AuditRepository auditrepository;

    public Page<AuditEntity> listByPage(int pageNo, int pageSize, AuditEntity entity) {
        Page<AuditEntity> findoutpage = null;
        Sort sort = Sort.by(Sort.Direction.DESC, "time");
        Pageable pageable = PageRequest.of(pageNo, pageSize, sort);
        ExampleMatcher matcher = ExampleMatcher.matching()
                .withStringMatcher(ExampleMatcher.StringMatcher.CONTAINING)  //改变默认字符串匹配为:模糊查询
                .withMatcher("role", ExampleMatcher.GenericPropertyMatchers.contains())  //name字段模糊匹配
//                .withMatcher("role", ExampleMatcher.GenericPropertyMatchers.startsWith())  //name字段开头模糊匹配
//                .withMatcher("role", ExampleMatcher.GenericPropertyMatchers.endsWith())  //name字段结尾模糊匹配
                .withIgnorePaths("id"); //忽略id字段
        Example<AuditEntity> ex = Example.of(entity, matcher); //动态查询
        findoutpage = auditrepository.findAll(ex, pageable);
        return findoutpage;
    }

    public long countByRole(String role) {
        long number = auditrepository.countByRole(role);
        return number;
    }

    public AuditEntity auditLogSave(LocalDateTime time,
                                    String ipaddress,
                                    String username,
                                    String role,
                                    String method,
                                    String module,
                                    String object,
                                    String result,
                                    String status) {
        //检查参数是否合法
        AuditEntity auditentity = new AuditEntity();
        auditentity.setTime(time);
        auditentity.setIpaddress(ipaddress);
        auditentity.setUsername(username);
        auditentity.setRole(role);
        auditentity.setMethod(method);
        auditentity.setModule(module);
        auditentity.setObject(object);
        auditentity.setResult(result);
        auditentity.setStatus(status);
        AuditEntity saveresult = auditrepository.save(auditentity);
        return saveresult;
    }


    public Page<AuditEntity> auditLogSearch(
            LocalDateTime starttime, LocalDateTime endtime, String username, String operatemodal,
            int pageNo, int pageSize, String role
    ) {
        Page<AuditEntity> findoutpage = null;
        Sort sort = Sort.by(Sort.Direction.DESC, "time");
        Pageable pageable = PageRequest.of(pageNo, pageSize, sort);

        if(0=="全部".compareToIgnoreCase(operatemodal)&&(null==username||username.length()<=0)){
            findoutpage = auditrepository.findAllByTimeAfterAndTimeBeforeAndRoleEquals(starttime, endtime, role, pageable);
        }else if((0=="安全审计员".compareToIgnoreCase(role)||
                0=="安全保密管理员".compareToIgnoreCase(role)||
                0=="系统管理员".compareToIgnoreCase(role)||
                0=="普通用户".compareToIgnoreCase(role)
                )&&(null==username||username.length()<=0)){
            findoutpage = auditrepository.findAllByTimeAfterAndTimeBeforeAndModuleEqualsAndRoleEquals(starttime, endtime, operatemodal, role, pageable);
        }else if(0=="全部".compareToIgnoreCase(operatemodal)&&null!=username&&username.length()>0){
            findoutpage = auditrepository.findAllByTimeAfterAndTimeBeforeAndUsernameEqualsAndRoleEquals(starttime, endtime, username, role, pageable);
        }else{
            findoutpage = auditrepository.findAllByTimeAfterAndTimeBeforeAndUsernameEqualsAndModuleEqualsAndRoleEquals(starttime, endtime, username, operatemodal, role, pageable);
        }
        return findoutpage;
    }

    public long countAuditLog(LocalDateTime starttime, LocalDateTime endtime, String username, String operatemodal, String role) {
        /*这里省略参数判断*/

        long number = 0 ;
        if(0=="全部".compareToIgnoreCase(operatemodal)&&(null==username||username.length()<=0)){
            number = auditrepository.countByTimeAfterAndTimeBeforeAndRoleEquals(starttime, endtime, role);
        }else if((0=="安全审计员".compareToIgnoreCase(operatemodal)||
                0=="安全保密管理员".compareToIgnoreCase(operatemodal)||
                0=="系统管理员".compareToIgnoreCase(operatemodal)||
                0=="普通用户".compareToIgnoreCase(operatemodal)
        )&&(null==username||username.length()<=0)){
            number = auditrepository.countByTimeAfterAndTimeBeforeAndModuleEqualsAndRoleEquals(starttime, endtime, operatemodal, role);
        }else if(0=="全部".compareToIgnoreCase(operatemodal)&&null!=username&&username.length()>0){
            number = auditrepository.countByTimeAfterAndTimeBeforeAndUsernameEqualsAndRoleEquals(starttime, endtime, username, role);
        }else{
            number = auditrepository.countByTimeAfterAndTimeBeforeAndUsernameEqualsAndModuleEqualsAndRoleEquals(starttime, endtime, username, operatemodal, role);
        }
//        long number = auditrepository.countByTimeAfterAndTimeBeforeAndUsernameEqualsAndModuleEqualsAndRoleEquals(starttime, endtime, username, operatemodal, role);
        return number;
    }
}
