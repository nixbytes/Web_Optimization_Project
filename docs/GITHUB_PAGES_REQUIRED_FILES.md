
---


# GitHub Pages Deployment Requirements
## Static Site Hosting File Checklist

**Branch Required:** `gh-pages-docs`  
**Git URL Format for Push:** `github.com/user/repository/refs/heads/gh-pages-docs`

---

### ✅ MINIMUM FILE INVENTORY FOR DEPLOYMENT

These files **MUST** exist on the `gh-pages-docs` branch at repository root:

```
index.html                          ← Main entry page (REQUIRED)
css/style.min.css                  ← Global styles (REQUIRED)  
js/perfmatters.min.js             ← Performance script (REQUIRED)
views/pizza.html                   ← Interactive demo (RECOMMENDED)
views/css/style.min.css            ← App-specific styles
views/js/main.js                  ← Application logic
views/js/pizza.js                 ← WASM bindings
```

**Alternative page files (optional feature demos):**
- `project-webperf.html`      → Performance guide demonstration
- `project-mobile.html`       → Mobile-first responsive version  
- `project-2048.html`        → 2048 game showcase

---

### 📊 Complete Static File Inventory

| Category | Files | Total Uncompressed Size | For GitHub Pages? |
|----------|-------|------------------------|-------------------|
| **HTML**  | index.html, project-*.html (3 files) | ~50KB | ✅ Core + ⚠️ Optional demos |
| **CSS**   | css/style.min.css, views/css/style.min.css | ~1.5KB | ✅ Required styling |
| **JS**    | js/perfmatters.min.js + views/js/ | ~3KB | ✅ Essential scripts |
| **Images** | Any referenced images (inline as data URI) | - | ⚠️ Optional, or inline |
| **Docs**  | GITHUB_PAGES_DEPLOYMENT.md in root | 1.2KB | ℹ️ Reference material |

---

### ❌ Files NOT Needed for Static Hosting

These files exist in repo but are build artifacts:

```bash
# C Source Code (compile-time only, never serves):
src/pizza.c
src/server.c

# Build Configuration (never served to browser):
Makefile
.git/                 # Git metadata
.gitmodules            # Git submodule config

# Generated Files:
views/js/pizza.wasm       # WASM binary - auto-loadable without server  
css/*.css.backup          # If any exist (cleanup before deploy)
```

---

### 🚀 Deployment Setup Instructions

**Step 1:** Create or checkout `gh-pages-docs` branch:

```bash
git checkout -b gh-pages-docs main
# OR if fresh deploy:
git checkout --orphan gh-pages-docs
# Commit only the static files listed above
git add index.html css/ js/ views/css/views/js/*.js
git commit -m "Deploy: static WebAssembly portfolio"
```

**Step 2:** Configure GitHub Pages (via Settings tab):
- Source: `gh-pages` branch (not gh-pages-docs) or use your branch name
- Root directory: `/` (root level, no prefix)
- Save changes

OR configure via Actions workflow:

```yaml
name: Deploy to GitHub Pages
on:
  push:
    branches: gh-pages-docs
    
jobs:
  deploy:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Deployment via GHA
        uses: peaceiris/actions-gh-pages@v3
        with:
          github_token: ${{ github.token }}
          publish_dir: '.'  # Everything from gh-pages-docs branch
  
```

---

### 🎯 Final Verification Checklist

Before pushing to `gh-pages-docs`, confirm all files exist:

```bash
git ls-tree --name-only -r gh-pages-docs | grep -E '\.(html|css|js)$' | sort
# Should list: 4+ HTML files, 2 CSS files, 3+ JS files total
```

Test URLs after deployment:
- ✅ `https://<username>.github.io/web-opt/` → Main portfolio
- ✅ `https://<username>.github.io/web-opt/index.html` → Entry page loads  
- ⚠️ `https://<username>.github.io/web-optviews/pizza.html` → Featured demo

---

**Last Updated:** November 17, 2026  
**Project:** Web Performance Optimization Demo  
**Deployment Branch:** `gh-pages-docs`  
