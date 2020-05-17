
package Mayus;

import javax.xml.bind.JAXBElement;
import javax.xml.bind.annotation.XmlElementDecl;
import javax.xml.bind.annotation.XmlRegistry;
import javax.xml.namespace.QName;


/**
 * This object contains factory methods for each 
 * Java content interface and Java element interface 
 * generated in the Mayus package. 
 * <p>An ObjectFactory allows you to programatically 
 * construct new instances of the Java representation 
 * for XML content. The Java representation of XML 
 * content can consist of schema derived interfaces 
 * and classes representing the binding of schema 
 * type definitions, element declarations and model 
 * groups.  Factory methods for each of these are 
 * provided in this class.
 * 
 */
@XmlRegistry
public class ObjectFactory {

    private final static QName _ToUpper_QNAME = new QName("http://Mayus/", "toUpper");
    private final static QName _ToUpperResponse_QNAME = new QName("http://Mayus/", "toUpperResponse");

    /**
     * Create a new ObjectFactory that can be used to create new instances of schema derived classes for package: Mayus
     * 
     */
    public ObjectFactory() {
    }

    /**
     * Create an instance of {@link ToUpper }
     * 
     */
    public ToUpper createToUpper() {
        return new ToUpper();
    }

    /**
     * Create an instance of {@link ToUpperResponse }
     * 
     */
    public ToUpperResponse createToUpperResponse() {
        return new ToUpperResponse();
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ToUpper }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://Mayus/", name = "toUpper")
    public JAXBElement<ToUpper> createToUpper(ToUpper value) {
        return new JAXBElement<ToUpper>(_ToUpper_QNAME, ToUpper.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ToUpperResponse }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://Mayus/", name = "toUpperResponse")
    public JAXBElement<ToUpperResponse> createToUpperResponse(ToUpperResponse value) {
        return new JAXBElement<ToUpperResponse>(_ToUpperResponse_QNAME, ToUpperResponse.class, null, value);
    }

}
