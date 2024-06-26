package com.sklois.SecureCloudStorageSystem.entity;


import jakarta.persistence.*;


import java.time.LocalDateTime;

@Entity //实体类注解
public class Metadata {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private Integer id;
    @Column(nullable = false)
    private String filename;
    private long filesize;
    private LocalDateTime ctime;
    private LocalDateTime mtime;
    private LocalDateTime lastaccesstime;
    private LocalDateTime uploadtime;
    @Column(nullable = false)
    private String fileowner;
    private String plaintexthash;
    private String ciphertexthash;
    private long ciphertextsize;
    private String keyhash;
    @Column(nullable = false)
    private String fileuniqueid;
    private String type;
    @Column(nullable = false)
    private String currentid;
    @Column(nullable = false)
    private String parentid;
    private boolean ifshared;

    private boolean ifopened;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getFilename() {
        return filename;
    }

    public void setFilename(String filename) {
        this.filename = filename;
    }

    public long getFilesize() {
        return filesize;
    }

    public void setFilesize(long filesize) {
        this.filesize = filesize;
    }

    public LocalDateTime getCtime() {
        return ctime;
    }

    public void setCtime(LocalDateTime ctime) {
        this.ctime = ctime;
    }

    public LocalDateTime getMtime() {
        return mtime;
    }

    public void setMtime(LocalDateTime mtime) {
        this.mtime = mtime;
    }

    public LocalDateTime getLastaccesstime() {
        return lastaccesstime;
    }

    public void setLastaccesstime(LocalDateTime lastaccesstime) {
        this.lastaccesstime = lastaccesstime;
    }

    public LocalDateTime getUploadtime() {
        return uploadtime;
    }

    public void setUploadtime(LocalDateTime uploadtime) {
        this.uploadtime = uploadtime;
    }

    public String getFileowner() {
        return fileowner;
    }

    public void setFileowner(String fileowner) {
        this.fileowner = fileowner;
    }

    public String getPlaintexthash() {
        return plaintexthash;
    }

    public void setPlaintexthash(String plaintexthash) {
        this.plaintexthash = plaintexthash;
    }

    public String getCiphertexthash() {
        return ciphertexthash;
    }

    public void setCiphertexthash(String ciphertexthash) {
        this.ciphertexthash = ciphertexthash;
    }

    public long getCiphertextsize() {
        return ciphertextsize;
    }

    public void setCiphertextsize(long ciphertextsize) {
        this.ciphertextsize = ciphertextsize;
    }

    public String getKeyhash() {
        return keyhash;
    }

    public void setKeyhash(String keyhash) {
        this.keyhash = keyhash;
    }

    public String getFileuniqueid() {
        return fileuniqueid;
    }

    public void setFileuniqueid(String fileuniqueid) {
        this.fileuniqueid = fileuniqueid;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getCurrentid() {
        return currentid;
    }

    public void setCurrentid(String currentid) {
        this.currentid = currentid;
    }

    public String getParentid() {
        return parentid;
    }

    public void setParentid(String parentid) {
        this.parentid = parentid;
    }

    public boolean isIfshared() {
        return ifshared;
    }

    public void setIfshared(boolean ifshared) {
        this.ifshared = ifshared;
    }

    public boolean isIfopened() {
        return ifopened;
    }

    public void setIfopened(boolean ifopened) {
        this.ifopened = ifopened;
    }
}
