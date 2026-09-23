import os
import re

files_to_convert = [
    'doc/license.txt',
    'doc/to_do.txt',
    'doc/RNG_usage.txt',
    'doc/Tests_overview.txt',
    'doc/Tests_results.txt'
]

def format_markdown(text):
    # Convert header lines like "1. Title" to "# 1. Title"
    text = re.sub(r'^(\d+\.\s+.*)$', r'# \1', text, flags=re.MULTILINE)
    
    # Convert sub-headers like "A. subtitle" to "## A. subtitle"
    text = re.sub(r'^([A-Z]\.\s+.*)$', r'## \1', text, flags=re.MULTILINE)
    
    # Convert "features:" or similar to "### features:"
    text = re.sub(r'^([a-zA-Z0-9_\s]+\:)$', r'### \1', text, flags=re.MULTILINE)
    
    # Convert "===" underlines to #
    text = re.sub(r'^(.*?)\n={5,}\s*$', r'# \1', text, flags=re.MULTILINE)
    text = re.sub(r'^={5,}\s*$', '', text, flags=re.MULTILINE)
    
    # Convert "---" underlines to ##
    text = re.sub(r'^(.*?)\n-{5,}\s*$', r'## \1', text, flags=re.MULTILINE)
    text = re.sub(r'^-{5,}\s*$', '', text, flags=re.MULTILINE)
    
    return text

for filepath in files_to_convert:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        md_content = format_markdown(content)
        
        out_path = filepath.replace('.txt', '.md')
        with open(out_path, 'w', encoding='utf-8') as f:
            f.write(md_content)
        print(f"Converted {filepath} to {out_path}")
        os.remove(filepath)
