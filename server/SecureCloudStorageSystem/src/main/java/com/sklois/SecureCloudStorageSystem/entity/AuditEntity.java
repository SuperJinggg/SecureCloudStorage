package com.sklois.SecureCloudStorageSystem.entity;


import jakarta.persistence.*;

import java.time.LocalDateTime;

@Entity //实体类注解
public class AuditEntity {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private Integer id;
    private LocalDateTime time; //时间
    private String ipaddress;//客户端IP地质
    private String username;//用户名
    private String role;//用户角色
    private String method;//操作方法,例如账户登录,账户退出,文件上传
    private String module;//模块,登录模块,文件浏览模块
    @Column(length = 2048)
    private String object;//访问资源,目标等,登录模块是xxxx端,文件上传是xxx文件
    private String result;//成功或者失败
    private String status;//状态,开始状态,执行状态,中断状态,异常状态,终止状态等.

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public LocalDateTime getTime() {
        return time;
    }

    public void setTime(LocalDateTime time) {
        this.time = time;
    }

    public String getIpaddress() {
        return ipaddress;
    }

    public void setIpaddress(String ipaddress) {
        this.ipaddress = ipaddress;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getRole() {
        return role;
    }

    public void setRole(String role) {
        this.role = role;
    }

    public String getMethod() {
        return method;
    }

    public void setMethod(String method) {
        this.method = method;
    }

    public String getModule() {
        return module;
    }

    public void setModule(String module) {
        this.module = module;
    }

    public String getObject() {
        return object;
    }

    public void setObject(String object) {
        this.object = object;
    }

    public String getResult() {
        return result;
    }

    public void setResult(String result) {
        this.result = result;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }
}
