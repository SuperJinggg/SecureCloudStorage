package com.sklois.SecureCloudStorageSystem.entity;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;

import java.time.LocalDateTime;

@Entity //实体类注解
public class RegisterUser {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private Integer id;
    private String name;
    private String password;
    private String role;
    private LocalDateTime registerdate ;//用户注册时间

    private String sessionid;
    private String sessionkey;
    private LocalDateTime sessiontime;//
//    private long sessioninactiveinterval;//session持续有效时间,单位s
    private LocalDateTime lastauthtime;//上次登录时间
    private String lastauthipaddress;//上次登录ipaddress


    private String userstatus; //激活,禁用,锁定等状态
    private LocalDateTime locktime;
    private String lockreason;//锁定原因
    private String lockedbywho;//被谁锁定




    public Integer getId() {
        return id;
    }
    public void setId(Integer id) {
        this.id = id;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }

    public String getPassword() {
        return password;
    }

    public String getRole() {
        return role;
    }

    public void setRole(String role) {
        this.role = role;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void setRegisterdate(LocalDateTime registerdate) {
        this.registerdate = registerdate;
    }

    public String getSessionid() {
        return sessionid;
    }

    public void setSessionid(String sessionid) {
        this.sessionid = sessionid;
    }

    public String getSessionkey() {
        return sessionkey;
    }

    public void setSessionkey(String sessionkey) {
        this.sessionkey = sessionkey;
    }

    public LocalDateTime getSessiontime() {
        return sessiontime;
    }

    public void setSessiontime(LocalDateTime sessiontime) {
        this.sessiontime = sessiontime;
    }

//    public long getSessioninactiveinterval() {
//        return sessioninactiveinterval;
//    }
//
//    public void setSessioninactiveinterval(long sessioninactiveinterval) {
//        this.sessioninactiveinterval = sessioninactiveinterval;
//    }

    public LocalDateTime getLastauthtime() {
        return lastauthtime;
    }

    public void setLastauthtime(LocalDateTime lastauthtime) {
        this.lastauthtime = lastauthtime;
    }

    public LocalDateTime getRegisterdate() {
        return registerdate;
    }
    public void setDate(LocalDateTime Registerdate) {
        this.registerdate = registerdate;
    }

    public String getLastauthipaddress() {
        return lastauthipaddress;
    }

    public void setLastauthipaddress(String lastauthipaddress) {
        this.lastauthipaddress = lastauthipaddress;
    }
}
