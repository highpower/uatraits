import ru.yandex.uatraits.*;
import java.util.*;

public class Test {
	private static final Detector detector = new Detector("../data/browser.xml");

	public static void main(String[] args) {
		for (Map.Entry<String, String> entry: detector.detect("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.89 Safari/537.1").entrySet()) {
			System.out.println(entry.getKey() + "=" + entry.getValue());
		}
		System.out.println(detector.detect(null));
	}

}
