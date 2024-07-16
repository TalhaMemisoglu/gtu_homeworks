import java.util.HashMap;
import java.util.Map;
import java.util.LinkedHashSet;
import java.util.Set;
import java.util.Iterator;

public class alphabet {
	private Set<Character> english_alphabet = new LinkedHashSet<Character>();
	private Map<Character, Map<Character, Character>> map = new HashMap<Character,  Map<Character, Character>>();
	
	public alphabet() {
		// do not edit this method
		fill_english_alphabet();
		fill_map();
	}
	
	private void fill_english_alphabet() {
		// do not edit this method
		for(char c : "ABCDEFGHIJKLMNOPQRSTUVWXYZ".toCharArray()) {
		    english_alphabet.add(c);
		}
	}
	
	private void fill_map() {
        Iterator<Character> rowIterator = english_alphabet.iterator();
		while(rowIterator.hasNext()){
            Map<Character, Character> rowMap = new HashMap<>();
			Iterator<Character> colIterator = english_alphabet.iterator();
			Character rowKey = rowIterator.next() , c = rowKey;
            while(colIterator.hasNext()){
                if(c != 'Z'){
                    rowMap.put(colIterator.next(), c);
                    c++;
                } else {//If c is Z, go back to beginning.
                    rowMap.put(colIterator.next(), c);
                    c = 'A';
                }
            }
            map.put(rowKey, rowMap);
        }
	}

	public void print_map() {
		// do not edit this method
		System.out.println("*** Viegenere Cipher ***\n\n");
		System.out.println("    " + english_alphabet);
		System.out.print("    ------------------------------------------------------------------------------");
		for(Character k: map.keySet()) {
			System.out.print("\n" + k + " | ");
			System.out.print(map.get(k).values());
		}
		System.out.println("\n");
	}

	public Map get_map() {
		return map;
	}
}