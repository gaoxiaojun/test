import java.io.Console;

public class ConsoleTest {
    public static void main(String[] args) {
        Console cons = System.console();
        String username = cons.readLine("User name: ");
        char[] passwd = cons.readPassword("Password: ");
        System.out.println("User name: " + username);
        System.out.println("Password: " + String.valueOf(passwd));
    }
}