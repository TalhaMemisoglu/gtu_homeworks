import java.util.Map;

public class encryptor {
	private Map<Character, Map<Character, Character>> map;
	private String key;
	private String keystream = "";
	private String plain_text;
	private String cipher_text = "";
	
	public encryptor(Map<Character, Map<Character, Character>> _map, String _key, String text) {
		map = _map;
		key = _key;
		plain_text = text;
	}
	
	public void encrypt() {
		// do not edit this method
		generate_keystream();
		generate_cipher_text();
	}
	
	private void generate_keystream() {
		if(plain_text.length() > key.length()){
			int indexKey = 0;
			for(int i=0; i < plain_text.length(); i++){
				keystream += key.charAt(indexKey++);
				if(indexKey == key.length())
					indexKey = 0;
			}
		} else if(plain_text.length() < key.length()){
			for(int i=0; i < plain_text.length(); i++){
				keystream += key.charAt(i);
			}
		} else /*plain_text.length() = key.length()*/ {
			keystream = key;
		}
	}
	
	private void generate_cipher_text() {		
		Map<Character,Character> rowMap;
		Character ch;
		int indexKey = 0;
		int indexTxt = 0;
		while(indexKey < keystream.length()){
			rowMap = map.get(plain_text.charAt(indexTxt));
			ch = rowMap.get(keystream.charAt(indexKey));
			cipher_text += ch;
			indexKey++;
			indexTxt++;
		}
	}

	public String get_keystream() {
		return keystream;
	}
	
	public String get_cipher_text() {
		return cipher_text;
	}
}
