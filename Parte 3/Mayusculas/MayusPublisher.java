package Mayusculas;

import javax.xml.ws.Endpoint;

public class MayusPublisher{
    public static void main(String[ ] args) {
        final String url = "http://localhost:8888/Mayus";
        System.out.println("Publishing ToUpperCase endpoint " + url);
        Endpoint.publish(url, new Mayus());
    }
} 
