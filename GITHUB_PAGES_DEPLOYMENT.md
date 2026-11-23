
---


## 🎯 Summary: Required Files for GitHub Pages Deployment

### ✅ Minimum Complete Set (Deploy These):

```bash
index.html                              # Main entry point
css/style.min.css                       # Global styles
js/perfmatters.min.js                  # Performance logging
views/pizza.html                       # Interactive demo (featured)
views/js/main.js                       # Event handlers
views/js/pizza.js                     # WASM bindings (required for pizza app)
```

### 📋 Detailed File Inventory:

| Type | Files | Purpose | Min Files |
|------|-------|---------|-----------|
| **HTML** | `index.html` + `project-*.html` | Main pages | 1+ (root or views/) |
| **CSS** | All `.css` files in root and `views/css/` | Styling | 1 per level |
| **JavaScript** | All `.js` files in root and `views/js/` | Logic + WASM glue | 1 core + 1 dynamic |
| **Images** | Static PNG/JPEG/SVG inline in HTML | Visuals | Optional (inline) |

### ⚡ Deployment Workflow:

```yaml
branch: gh-pages-docs                 # Must use this branch name
source_dir: ./                        # Root contains all needed files
publish_via: GitHub Settings          OR Actions workflow YAML
```

---
