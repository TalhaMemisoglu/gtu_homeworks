import java.util.Map;
import java.util.Iterator;

public class decryptor {
	private Map<Character, Map<Character, Character>> map;
	private String key;
	private String keystream = "";
	private String plain_text = "";
	private String cipher_text;
	
	public decryptor(Map<Character, Map<Character, Character>> _map, String _key, String text) {
		map = _map;
		key = _key;
		cipher_text = text;
	}

	public void decrypt() {
		// do not edit this method
		generate_keystream();
		generate_plain_text();
	}
	
	private void generate_keystream() {
		if(cipher_text.length() > key.length()){
			int indexKey = 0;
			for(int i=0; i < cipher_text.length(); i++){
				keystream += key.charAt(indexKey++);
				if(indexKey == key.length())
					indexKey = 0;
			}
		} else if(cipher_text.length() < key.length()){
			for(int i=0; i < cipher_text.length(); i++){
				keystream += key.charAt(i);
			}
		} else /*plain_text.length() = key.length()*/ {
			keystream = key;
		}
	}
	
	private void generate_plain_text() {
		// You must use map.get(x).keySet() with an iterator in this method
		for(int indexKey=0, indexTxt=0; indexKey<keystream.length(); indexKey++,indexTxt++)
			for(Character c='A'; c <= 'Z'; c++){
				Iterator<Character> keyIterator = map.get(c).keySet().iterator();//get Iterator from keySet.
				boolean found = false;
				while(keyIterator.hasNext()){
					Character ch = keyIterator.next();
					if(ch == keystream.charAt(indexKey)){
						if(cipher_text.charAt(indexTxt) == map.get(c).get(ch)){
							plain_text += c;
							found = true;
							break;
						}
					}
				}
				if(found)//If key is found, exit the loop
					break;
			}
	}

	public String get_keystream() {
		return keystream;
	}
	
	public String get_plain_text() {
		return plain_text;
	}
}
