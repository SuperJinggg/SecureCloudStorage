package com.sklois.SecureCloudStorageSystem.service;

import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;

import java.util.List;

public interface UserListSer {

    public long countByRole(String role);
    public List<RegisterUser> listByRole(String role);

}
