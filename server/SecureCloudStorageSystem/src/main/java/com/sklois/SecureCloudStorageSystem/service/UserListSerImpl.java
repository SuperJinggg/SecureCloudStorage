package com.sklois.SecureCloudStorageSystem.service;

import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import com.sklois.SecureCloudStorageSystem.repository.RegisterUserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class UserListSerImpl implements UserListSer{

    @Autowired
    RegisterUserRepository userlistrepository;

    public long countByRole(String role){
        long number = userlistrepository.countByRole(role);
        return number;
    }
    public List<RegisterUser> listByRole(String role){
        List<RegisterUser> userlist = userlistrepository.findByRole(role);
        return userlist;

    }

}
