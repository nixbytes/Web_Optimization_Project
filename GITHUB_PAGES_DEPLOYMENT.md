# GitHub Pages Deployment Guide

> **Purpose**: Documentation for deploying this WebAssembly-based web project as a static website using GitHub Pages hosting.

---

## 📋 Table of Contents

1. [Required Files Overview](#required-files-overview)
2. [File Inventory](#file-inventory)
3. [GitHub Pages Configuration Setup](#github-pages-configuration-setup)
4. [Deployment Verification Checklist](#deployment-verification-checklist)
5. [Static Hosting Limitations & Workarounds](#static-hosting-limitations--workarounds)

---

## Required Files Overview

This project can be deployed as a static website since it uses vanilla HTML/CSS/JavaScript with WebAssembly module loaded via script tag. Here are the **required file types** for GitHub Pages (and any static host):

### Core Required Files

| File Type | Ext(s) | Purpose | Location | Min Files Required |
|-----------|--------|---------|----------|-----|
| HTML Entry Point | `.html` | Main page/index | Root or `/views/` | 1 |
| CSS Stylesheets | `.css` | Styling | `/css/` or `/views/css/` | 1+ |
| JavaScript Logic | `.js` | DOM manipulation, WASM loader | `/js/` or `/views/js/` | 1+ |

### Optional but Helpful Files

| File Type | Ext(s) | Purpose | Recommendation |
|-----------|--------|---------|---------------|
| WebAssembly Module | `.wasm` + `.js` | Binary execution (if needed) | Build once, deploy all |
| Images/Media | `.png`, `.jpg`, `.gif` | Visual assets | Optimize for web |
| Favicon | `.ico` or `.svg` | Browser tab icon | 1 recommended |

---

## File Inventory

Based on analysis of this repository:

### 🎯 Deployable Core Files (Minimum Complete Set)

```
index.html                                    ✓ Entry point
css/style.min.css                            ✓ Global styles
js/perfmatters.min.js                        ✓ Main logic
views/pizza.html                             ✓ Pizza demo page
views/css/style.min.css                      ✓ View-specific styles
views/js/main.js                             ✓ View scripts
images/                                      (folder) Optional assets/docs
```

### ⚡ WebAssembly Module Files

The `pizza.wasm` and its companion `.js` glue file are generated but required for the functionality:

```
src/          # Source C files (NOT needed - just references)
views/js/pizza.js    ✓ Required for WASM loading
views/js/pizza.wasm  ✓ Optional if not loaded directly
```

### 📁 All Available Pages in Repository

| File | Type | Description | Deployable? |
|------|------|-------------|-------------|
| `index.html` | HTML | Landing page with stats/features | ✅ Yes |
| `project-webperf.html` | HTML | Web performance demo page | ✅ Yes |
| `project-mobile.html` | HTML | Mobile-first version | ✅ Yes |
| `project-2048.html` | HTML | 2048 game demonstration | ✅ Yes |
| `views/pizza.html` | HTML | Pizza generator app | ✅ Yes - Feature page |

### 🎨 CSS Files (All Locations)

| Path | File | Status |
|------|------|--------|
| `/css/style.min.css` | Main stylesheet (minified) | ✅ Deploy with JS |
| `/views/css/style.min.css` | View-specific styles (minified) | ✅ Required if views/ deployed |
| `src/css/bootstrap-grid.css` | Bootstrap grid framework | ℹ️ Optional CSS reset |

### 📖 JavaScript Files (All Locations)

| Path | File | Description |
|------|------|-------------|
| `/js/perfmatters.js` | Full source | ✅ Keep for debugging |
| `/js/perfmatters.min.js` | Minified prod | ✅ Deploy this for production |
| `/views/js/main.js` | Main view script | ✅ Required |
| `views/js/pizza.js` | Generated from pizza.c | ✅ WASM interface |

---

## GitHub Pages Configuration Setup

### Option 1: Using `pages` Branch (Recommended)

**Step 1:** Create a new branch named exactly `gh-pages` or `main`:

```bash
git checkout -b gh-pages
cp -r /Users/nix/Documents/Code_Projects/Web_Optimization_Project/* . 
cp -r views .
git add .
git commit -m "static: prepare all files for GitHub Pages"
git push origin gh-pages
```

**OR** if you want to keep `main` as the source:

**Step 1:** Create `gh-pages` branch from current state:

```bash
git checkout -b gh-pages
# Remove build artifacts not needed for static hosting
find . -name "*.wasm" -exec rm {} \;    # WASM files only if not loaded via script
git add .
git commit -m "prepare: exclude wasm binary, keep js glue code"
```

### Option 2: Using `_config.yml` Configuration File

Create `.github/pages/.yml` configuration:

```yaml
source: docs         # Where your static files live (root or views/)
baseurl: ""          # Empty if hosting at username.github.io/user/project/
plugins:
  - jekyll
```

### GitHub Settings Configuration Path:

1. Go to repository **Settings** → **Pages** tab
2. Source selection dropdown: choose `gh-pages` branch (or leave on main)
3. Root directory: `/` for root-level deployment OR `/views` for views subdirectory
4. Save changes

---

### Recommended Structure for Deployment

```
root/                            ← GitHub pages root
  index.html                     ✓ Landing page
  css/
    style.min.css               ✓ Main styles
  js/
    perfmatters.min.js          ✓ Main logic (minified)
  views/                         ← Feature app directory
    pizza.html                  ✓ Pizza generator
    css/
      style.min.css            ✓ View-specific CSS
    js/
      main.js                   ✓ Entry point scripts
      pizza.js                 ✓ WASM bindings
```

---

## Deployment Verification Checklist

Before pushing to `gh-pages` branch, verify each item:

### HTML Files ✅

- [ ] `index.html` contains `<head>` section with proper meta tags
- [ ] All `.html` files have accessible viewport meta tag: `<meta name="viewport" content="width=device-width">`
- [ ] No relative paths broken (uses `/absolute/path/` or root-level)
- [ ] Links to CSS: `<link rel="stylesheet" href="/css/style.min.css">`
- [ ] Links to JS: `<script src="/js/perfmatters.min.js"></script>`

### JavaScript Files ✅

- [ ] `perfmatters.min.js` exists and is valid JavaScript
- [ ] No server-side code (C code paths should be pre-built)
- [ ] WASM module loaded via `<script>` tag properly configured:
  ```html
  <script src="/js/pizza.js"><\/script>
  <script>
    // Initialize pizza.c functions when DOM ready
    console.log("Pizza WASM initialized");
  </script>
  ```

### CSS Files ✅

- [ ] All minified `.css` files parse correctly in browser dev tools
- [ ] No `url()` references to missing assets
- [ ] No server-dependent styles (e.g., `@import` from non-existent paths)

### Build System Check ✅

```bash
# Verify the static-only build works without Emscripten
make test
# Should pass - tests validate JS behavior, not build artifacts
```

---

### Deployment Test URL Patterns:

After deployment to `https://<username>.github.io/<repo>/`:

- Landing page: `https://<username>.github.io/<repo>`
- Feature app: `https://<username>.github.io/<repo>/views/pizza.html`

If using `/views/` as root with `baseurl: /views`:
- Feature app: `https://<username>.github.io/<repo>/views/pizza.html`

---

## Static Hosting Limitations & Workarounds

### ⚠️ WebAssembly Binary Constraints

GitHub Pages is **static-only hosting**. This means:

| Aspect | Situation | Workaround/Status |
|--------|-----------|-------------------|
| Binary WASM Files (`.wasm`) | Cannot be directly loaded by browsers via server-side processing | ✅ Not an issue if JS glue code loads WASM browser-side via script tag |
| Server-Side Scripts (.py, .php) | Will return 404 if referenced from static pages | Ensure all logic is in `.js` files only |
| Background Workers | Supported but must be pre-existing (cannot fetch dynamically without build) | N/A for this project |

### Current Implementation Compatibility:

✅ **Already compatible:**
- All JS code is browser-accessible via `<script src="...">`
- WebAssembly module initialized from JavaScript glue code (no direct binary HTTP fetch needed from server)

⚠️ **Consider before pushing to gh-pages:**
- Ensure `pizza.wasm` file can be cached and served directly (it's a static asset that can work without special backend)
- WASM modules typically need `Cross-Origin-Resource-Sharing` headers for certain hosting setups, but on same-origin GitHub Pages: should work

### File Size Optimization Checklist for Hosting:

Run this to verify compression:

```bash
# Check file sizes (target: < 100kb per asset)
find . -name "*.html" -o -name "*.css" -o -name "*.js" | xargs du -h

# Expected breakdown after initial deployment:
# index.html        (~26KB, minifiable to ~8KB)
# project-webperf.html (~16KB, minifiable)
# perfmatters.min.js    (~25KB uncompressed, needs minification) 
```

**Action Items for Production Deployment:**

1. Run production build (minified JS, optimized CSS) - already done ✅
2. Consider using GitHub Pages **Build Plugin** if additional optimization needed:
   ```yaml
   plugins:
     jekyll-assets: true  # Process .html files through Jekyll for minification
   ```

---

## Quick Deployment Commands

### Full Production Push to GitHub Pages:

```bash
git checkout -b gh-pages HEAD
# Copy all static files needed for browser-side execution only
cp index.html .
cp -r css views/ js/ images/ 2>/dev/null || true

# Verify no build-only artifacts committed
git status  # Should show clean or only HTML/CSS/JS changes

git add .
git commit -m "docs: prepare static assets for GitHub Pages hosting"
git push origin gh-pages
```

### Via Web UI Approach (Simpler):

1. Create new branch named `gh-pages` in GitHub website (under **Settings > Branches**)
2. Go to the branch tab
3. Add files from root `/` with:
   - All `.html` files
   - All `.css` files  
   - All `.js` files needed for static deployment

4. Save and wait 1-2 minutes for index.html generation

---

## File Mapping Summary

### Input Repository → GitHub Pages Structure

```
src/                  → [HIDDEN, DO NOT INCLUDE]        (C source files)
index.html            → index.html                       ✓ Deploy
css/style.min.css     → css/style.min.css                ✓ Deploy  
js/perfmatters.js     → js/perfmatters.min.js           ✓ Deploy (rename to .min)
views/                → views/                          ✓ Keep as subdirectory

# Generated build artifacts (Emscripten output) - Optional: exclude from gh-pages:
views/js/pizza.wasm   → [EXCLUDE]                        (static binary, works automatically)
```

### Dependencies Note

No third-party package dependencies need resolution for GitHub Pages hosting:

```json
// No package.json entries needed - project runs as static HTML files with inline script references
```

Project is completely self-contained and requires no npm install/build step during deployment.

---

## Security Considerations

### Content Security Policy (CSP) For Static Files

Add meta tag to `index.html` `<head>` section:

```html
<meta http-equiv="Content-Security-Policy" 
      content="default-src 'self'; 
               script-src 'self' 'unsafe-inline';
               style-src 'self' 'unsafe-inline';">
```

Or configure via GitHub Pages `_config.yml`:

```yaml
header_pages_only: true  # Allow static pages to serve
exclude_patterns: []     # No filtering
```

### Permissive CSP (Development Mode) - Recommended for Demo Sites:

GitHub Pages typically permits safe inline scripts automatically. For WebAssembly modules running in browser context, no additional headers needed on same-origin GitHub Pages deployment.

---

## Troubleshooting Deployment

### 404 Not Found Errors:

**Cause:** Relative paths broken or incorrect baseurl configuration

**Fix:** Verify all asset references start from `/` (root) not relative paths without slash:

Wrong (will break with custom subdirectory):
```html
<link rel="stylesheet" href="css/style.css">
<!-- becomes /custom/path/css/style.css when accessed normally -->
```

Correct:
```html
<link rel="stylesheet" href="/css/style.min.css">
<script src="/js/perfmatters.min.js"><\/script>
```

### 403 Forbidden Errors:

**Cause:** Large asset size or invalid Content-Type headers for binary files

**Fix:** Ensure `.wasm` and `.js` have proper MIME types in `_config.yml`:

```yaml
defaults:
  - scope:
      path: /views/js/
    values:
      custom_css_extentions: []
    custom_suffixes: []
```

If using Jekyll build plugin, add `custom_files` config for proper mime/types.

### WebAssembly Module Loading Issues:

If browser logs show:
```
Error: Module doesn't export [memory|env]
```

**Cause:** WASM module expects server-provided HTTP access but GitHub Pages treats `.wasm` as regular binary download

**Fix:** Ensure `.wasm` file is accessible via direct link in generated script bindings, or recompile without `--no-emrun`.

---

## Verification Commands After Deployment

```bash
# Test if HTML files would parse correctly (syntax check)
python3 -c "import urllib.request; import html.parser as hp" 2>/dev/null || echo "Need to manually visit"
curl https://<username>.github.io/<repo>/index.html | grep -i "<html\|<head"

# Verify asset paths resolve correctly:
curl https://<username>.github.io/<repo>/css/style.min.css  > /dev/null && echo "CSS OK" || echo "CSS MISSING"
curl https://<username>.github.io/<repo>/js/perfmatters.min.js > /dev/null && echo "JS OK" || echo "JS MISSING"
```

---

## Support Resources

- [GitHub Pages Documentation](https://pages.github.com/)
- [GitHub Pages Help Center](https://help.github.com/categories/github-pages-basics/)
- [WebAssembly GitHub Issue Tracker](https://github.com/WebAssembly/wasm/issues)
- [CSP Specification for WebAssembly](https://www.w3.org/TR/csp2/#webassembly)

---

## Summary: Minimum Required Files

To deploy this project to GitHub Pages, these files are **necessary and sufficient**:

### Core HTML Document Structure ✅

```
index.html                            # Main landing page entry point
```

### Stylesheet (CSS) ✅

```
css/style.min.css                     # Global styles (minified production build)
```

### JavaScript Files ✅

```
js/perfmatters.min.js                # Optimized script code
views/js/main.js                      # DOM event handlers
views/js/pizza.js                     # WASM bindings generated by build
```

### Optional Visual Assets (Recommended but Not Required) ❓

```
static/                              # If you want custom images/favicons
  favicon.ico                        # Optional tab icon
```

---

**Last Updated:** November 16, 2024  
**Branch:** `gh-pages-docs`  
**Version:** Static Deployment V1.0
