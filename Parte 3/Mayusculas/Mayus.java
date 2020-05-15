package Mayusculas;

import javax.jws.WebService;
import javax.jws.WebMethod; 
import java.util.*;

@WebService
public class Mayus {
    @WebMethod
    public String toUpper(String desc) {
        String upper = desc;
        upper.toUpperCase();
        
        return upper;
    }
}
