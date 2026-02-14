import requests
import sys
from collections import Counter

# Ensure proper UTF-8 output for Hindi or any non-ASCII text
try:
    sys.stdout.reconfigure(encoding='utf-8')
except Exception:
    pass  # In case some environments don't support reconfigure()

def translate_text(source_lang, target_lang, text):
    """
    Translate text using MyMemory API.
    Handles Unicode and prevents auto language misdetection.
    """
    base_url = "https://api.mymemory.translated.net/get"
    params = {
        "q": text,
        "langpair": f"{source_lang}|{target_lang}",
        "de": "project@example.com"
    }

    try:
        response = requests.get(base_url, params=params, timeout=10)
        data = response.json()
        translated = data['responseData']['translatedText']
        return translated
    except Exception as e:
        return f"Error: {str(e)}"

def show_supported_languages():
    print("Supported language codes include:")
    print("en: English, hi: Hindi, fr: French, es: Spanish, de: German, it: Italian, ja: Japanese, ko: Korean, zh: Chinese")
    print("(You can add more according to MyMemory documentation.)")

def count_char_frequency(text):
    """Returns a dictionary of character frequencies."""
    return Counter(text)

def print_char_frequency(label, text):
    """Prints frequency of each character in a formatted way."""
    freq = count_char_frequency(text)
    print(f"\nCharacter Frequency in {label}:")
    print("-" * 40)
    for char, count in sorted(freq.items()):
        display_char = char if char != " " else "[space]"
        print(f"{display_char!r}: {count}")
    print("-" * 40)
    print(f"Total unique characters: {len(freq)}\n")

def main():
    history = []

    while True:
        print("\n--- MyMemory Translator ---")
        print("1. Translate text")
        print("2. View history")
        print("3. Show supported languages")
        print("4. Exit")

        try:
            option = int(input("Choose an option: "))
        except ValueError:
            print("Invalid input. Enter a number from 1 to 4.")
            continue

        if option == 1:
            source_lang = input("Source language code: ").strip().lower()
            target_lang = input("Target language code: ").strip().lower()
            text = input("Enter text: ")

            translated = translate_text(source_lang, target_lang, text)

            print("\n--- Translation Result ---")
            print(f"Source ({source_lang}): {text}")
            print(f"Target ({target_lang}): {translated}")

            # 🧮 Character count
            print(f"\nCharacter count:")
            print(f"Original text length: {len(text)}")
            print(f"Translated text length: {len(translated)}")

            # 🔠 Frequency of characters
            print_char_frequency("Original Text", text)
            print_char_frequency("Translated Text", translated)

            # 📝 Save translation history
            history.append((text, translated))
            try:
                with open("translation_history.txt", "a", encoding="utf-8") as f:
                    f.write(f"{source_lang}->{target_lang}: {text} => {translated}\n")
            except Exception as e:
                print(f"Error writing history: {e}")

        elif option == 2:
            print("\n--- Translation History ---")
            if not history:
                print("No translations yet.")
            else:
                for i, (src, trans) in enumerate(history, start=1):
                    print(f"{i}. {src} => {trans}")

        elif option == 3:
            show_supported_languages()

        elif option == 4:
            print("Exiting translator. Goodbye!")
            break

        else:
            print("Invalid option. Try again.")

if __name__ == "__main__":
    main()
