package com.sklois.SecureCloudStorageSystem.service;

import org.springframework.web.multipart.MultipartFile;

import java.io.IOException;
import java.io.InputStream;

public interface DockingCloudStorageSer {
    public void uploaddata(MultipartFile file,String fileuniqueid) throws IOException;
    public InputStream downloaddata(String fileuniqueid);
    public void  deletedata(String fileuniqueid);
}
