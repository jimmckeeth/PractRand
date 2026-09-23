import os
import re

def fix_notes(filepath):
    if not os.path.exists(filepath): return
    with open(filepath, 'r', encoding='utf-8') as f:
        text = f.read()
    
    # List of replacements (target, replacement)
    replacements = [
        (
            r'\*\s*\(Note:\s*(.*?)\)\*',
            r'> [!NOTE]\n> \1'
        ),
        (
            r'^\s*\*\s*Note:\s*(.*?)$',
            r'> [!NOTE]\n> \1'
        ),
        (
            r'^\s*-\s*\*Empirical Note:\*\s*(.*?)$',
            r'- > [!NOTE]\n  > Empirical Note: \1'
        ),
        (
            r'^\s*note:\s*(.*?)$',
            r'> [!NOTE]\n> \1'
        )
    ]
    
    new_text = text
    for patt, repl in replacements:
        new_text = re.sub(patt, repl, new_text, flags=re.MULTILINE)
        
    if text != new_text:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_text)
        print(f"Fixed notes in {filepath}")

for root, _, files in os.walk('doc'):
    for file in files:
        if file.endswith('.md'):
            fix_notes(os.path.join(root, file))
