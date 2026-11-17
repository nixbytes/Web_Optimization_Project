# 📁 Documentation Directory

This directory contains documentation files for the Web Optimization Project. 

### Purpose
- Contains markdown documentation, changelogs, and guides
- Excluded from static site deployment
- Used for project reference by developers

### File Overview

```
docs/
└── readme.md                     # This file - docs readme
├── PUSH_STATUS.md                # Build status tracking (excluded from gh-pages)
├── GITHUB_WIKI_README.md         # GitHub wiki template (excluded from gh-pages)
```

### Deployment Notes

This `docs/` directory is **excluded** from automatic GitHub Pages deployment via workflow path filtering (`paths-ignore`). 

The content here is meant for:
- Internal project documentation
- Build status tracking  
- Wiki references

Not intended as the root of deployable static site. Access these docs via Git repository browsing or use `_config.yml` in Jekyll source to serve from `/docs/`.

---

## Alternative: Using `_config.yml` for Local GitHub Pages Site

If you prefer keeping `gh-pages-docs` branch content, create configuration file at `.github/pages/.yml` (not standard - use `.github/GITHUB_PAGES_CONFIG.yml` or main root):

```yaml
# Alternative 2 Configuration (non-standard location)
repo_name: Web_Optimization_Project
source: .                     # Root directory
baseurl: ""                   # No prefix in URL
    
# Jekyll Settings
theme: null                  # Remove theme - deploy as plain static HTML
strict_front_matter: true    # Prevent non-Jekyll markdown files from processing
incremental: false           # Build fresh on each deployment

# Plugins Configuration
plugins:
  - jekyll-assets            # Enable .html file processing for static site
  
# Build Options
defaults:
  - scope:
      path: "views/"         # Process views/ as separate directory  
    values:
      layout: page          # Optional Jekyll layouts (remove if not using)
```

**Note:** Standard Jekyll builds require `_config.yml` in root. For GitHub Actions deployment with `peaceiris/actions-gh-pages`, configuration happens via workflow action parameters instead of config files.
