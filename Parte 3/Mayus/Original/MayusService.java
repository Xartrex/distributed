package Mayus;

import javax.jws.WebService;
import javax.jws.WebMethod; 
import java.util.*;

@WebService
public class MayusService {
    @WebMethod
    public String toUpper(String desc) {
        desc.toUpperCase();
        
        return desc;
    }
}
