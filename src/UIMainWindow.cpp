#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/filedlg.h>
#include <wx/filepicker.h>
#include <wx/image.h>
#include <wx/statbmp.h>
#include "InventoryManager.hpp"
#include "FileHandler.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <unordered_map>

const std::string DATA_FILE = "inventory.txt";
const std::string IMAGE_MAP_FILE = "inventory_images.txt";
const std::string WAREHOUSE_MAP_FILE = "inventory_warehouses.txt";
const std::string LOCATION_MAP_FILE = "inventory_locations.txt";

// Custom Modal Dialog for Adding New Products with Warehouse, Location & Image
class AddProductDialog : public wxDialog {
private:
    wxTextCtrl* idCtrl;
    wxTextCtrl* nameCtrl;
    wxTextCtrl* priceCtrl;
    wxTextCtrl* qtyCtrl;
    wxTextCtrl* warehouseCtrl;
    wxTextCtrl* locationCtrl;
    wxFilePickerCtrl* imagePickerCtrl;

public:
    AddProductDialog(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, "Add New Product", wxDefaultPosition, wxDefaultSize) {

        SetBackgroundColour(wxColour(30, 30, 46));

        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        wxFlexGridSizer* gridSizer = new wxFlexGridSizer(7, 2, 10, 10);
        gridSizer->AddGrowableCol(1, 1);

        auto addLabel = [this, gridSizer](const wxString& text) {
            wxStaticText* lbl = new wxStaticText(this, wxID_ANY, text);
            lbl->SetForegroundColour(wxColour(248, 248, 242));
            gridSizer->Add(lbl, 0, wxALIGN_CENTER_VERTICAL);
        };

        addLabel("Product ID:");
        idCtrl = new wxTextCtrl(this, wxID_ANY);
        gridSizer->Add(idCtrl, 1, wxEXPAND);

        addLabel("Product Name:");
        nameCtrl = new wxTextCtrl(this, wxID_ANY);
        gridSizer->Add(nameCtrl, 1, wxEXPAND);

        addLabel("Price (R):");
        priceCtrl = new wxTextCtrl(this, wxID_ANY);
        gridSizer->Add(priceCtrl, 1, wxEXPAND);

        addLabel("Stock Quantity:");
        qtyCtrl = new wxTextCtrl(this, wxID_ANY);
        gridSizer->Add(qtyCtrl, 1, wxEXPAND);

        addLabel("Warehouse:");
        warehouseCtrl = new wxTextCtrl(this, wxID_ANY);
        gridSizer->Add(warehouseCtrl, 1, wxEXPAND);

        addLabel("Location (Aisle/Bin):");
        locationCtrl = new wxTextCtrl(this, wxID_ANY);
        gridSizer->Add(locationCtrl, 1, wxEXPAND);

        addLabel("Product Image:");
        imagePickerCtrl = new wxFilePickerCtrl(
            this, wxID_ANY, "", "Select Image",
            "Image files (*.png;*.jpg;*.jpeg;*.bmp)|*.png;*.jpg;*.jpeg;*.bmp",
            wxDefaultPosition, wxDefaultSize, wxFLP_OPEN | wxFLP_FILE_MUST_EXIST
        );
        gridSizer->Add(imagePickerCtrl, 1, wxEXPAND);

        mainSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 15);

        wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
        if (buttonSizer) {
            mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 10);
        }

        SetSizerAndFit(mainSizer);
        CentreOnParent();
    }

    int GetID() const { return std::stoi(idCtrl->GetValue().ToStdString()); }
    std::string GetProductName() const { return nameCtrl->GetValue().ToStdString(); }
    double GetPrice() const { return std::stod(priceCtrl->GetValue().ToStdString()); }
    int GetQuantity() const { return std::stoi(qtyCtrl->GetValue().ToStdString()); }
    std::string GetWarehouse() const { return warehouseCtrl->GetValue().ToStdString(); }
    std::string GetLocation() const { return locationCtrl->GetValue().ToStdString(); }
    std::string GetImagePath() const { return imagePickerCtrl->GetPath().ToStdString(); }
};

// Custom Modal Dialog for Updating Stock Quantity
class UpdateStockDialog : public wxDialog {
private:
    wxTextCtrl* qtyCtrl;

public:
    UpdateStockDialog(wxWindow* parent, int currentQty)
        : wxDialog(parent, wxID_ANY, "Update Stock Quantity", wxDefaultPosition, wxDefaultSize) {

        SetBackgroundColour(wxColour(30, 30, 46));

        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* promptLbl = new wxStaticText(this, wxID_ANY, "Enter New Stock Quantity:");
        promptLbl->SetForegroundColour(wxColour(248, 248, 242));
        mainSizer->Add(promptLbl, 0, wxALL, 10);

        qtyCtrl = new wxTextCtrl(this, wxID_ANY, std::to_string(currentQty));
        mainSizer->Add(qtyCtrl, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

        wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
        if (buttonSizer) {
            mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 10);
        }

        SetSizerAndFit(mainSizer);
        CentreOnParent();
    }

    int GetQuantity() const { return std::stoi(qtyCtrl->GetValue().ToStdString()); }
};

class UIMainWindow : public wxFrame {
private:
    InventoryManager manager;
    FileHandler fileHandler;
    wxGrid* inventoryGrid;
    wxTextCtrl* searchCtrl;
    wxStaticText* summaryLabel;
    wxStaticBitmap* imagePreviewCtrl;

    std::unordered_map<int, std::string> productImages;
    std::unordered_map<int, std::string> productWarehouses;
    std::unordered_map<int, std::string> productLocations;
    int sortCol = 0;
    bool sortAscending = true;

    void SaveMetadataMaps() {
        // Save Images
        {
            std::ofstream file(IMAGE_MAP_FILE);
            if (file.is_open()) {
                for (const auto& pair : productImages) {
                    file << pair.first << "|" << pair.second << "\n";
                }
            }
        }
        // Save Warehouses
        {
            std::ofstream file(WAREHOUSE_MAP_FILE);
            if (file.is_open()) {
                for (const auto& pair : productWarehouses) {
                    file << pair.first << "|" << pair.second << "\n";
                }
            }
        }
        // Save Locations
        {
            std::ofstream file(LOCATION_MAP_FILE);
            if (file.is_open()) {
                for (const auto& pair : productLocations) {
                    file << pair.first << "|" << pair.second << "\n";
                }
            }
        }
    }

    void LoadMetadataMaps() {
        productImages.clear();
        productWarehouses.clear();
        productLocations.clear();

        // Load Images
        {
            std::ifstream file(IMAGE_MAP_FILE);
            if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    size_t delim = line.find('|');
                    if (delim != std::string::npos) {
                        int id = std::stoi(line.substr(0, delim));
                        productImages[id] = line.substr(delim + 1);
                    }
                }
            }
        }
        // Load Warehouses
        {
            std::ifstream file(WAREHOUSE_MAP_FILE);
            if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    size_t delim = line.find('|');
                    if (delim != std::string::npos) {
                        int id = std::stoi(line.substr(0, delim));
                        productWarehouses[id] = line.substr(delim + 1);
                    }
                }
            }
        }
        // Load Locations
        {
            std::ifstream file(LOCATION_MAP_FILE);
            if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    size_t delim = line.find('|');
                    if (delim != std::string::npos) {
                        int id = std::stoi(line.substr(0, delim));
                        productLocations[id] = line.substr(delim + 1);
                    }
                }
            }
        }
    }

    void SaveData() {
        fileHandler.saveInventory(DATA_FILE, manager.getAllProducts());
        SaveMetadataMaps();
    }

    void LoadData() {
        fileHandler.loadInventory(DATA_FILE, manager);
        LoadMetadataMaps();
    }

    void UpdateSummaryPanel(const std::vector<Product>& products) {
        double totalValue = 0.0;
        int lowStockCount = 0;

        for (const auto& prod : products) {
            totalValue += (prod.getPrice() * prod.getQuantity());
            if (prod.getQuantity() <= 5) {
                lowStockCount++;
            }
        }

        wxString summaryText = wxString::Format(
            "Total Products: %zu   |   Total Valuation: R %.2f   |   Low Stock Alerts (<=5): %d",
            products.size(), totalValue, lowStockCount
        );
        summaryLabel->SetLabel(summaryText);
    }

    void DisplayProductImage(int productId) {
        auto it = productImages.find(productId);
        if (it != productImages.end() && !it->second.empty()) {
            wxImage img;
            if (img.LoadFile(it->second)) {
                img.Rescale(200, 200, wxIMAGE_QUALITY_HIGH);
                imagePreviewCtrl->SetBitmap(wxBitmap(img));
                imagePreviewCtrl->Refresh();
                return;
            }
        }
        imagePreviewCtrl->SetBitmap(wxNullBitmap);
        imagePreviewCtrl->Refresh();
    }

    void PopulateGrid(const std::string& filterQuery = "") {
        if (inventoryGrid->GetNumberRows() > 0) {
            inventoryGrid->DeleteRows(0, inventoryGrid->GetNumberRows());
        }

        auto products = manager.getAllProducts();
        UpdateSummaryPanel(products);

        std::sort(products.begin(), products.end(), [this](const Product& a, const Product& b) {
            if (sortCol == 0) return sortAscending ? a.getId() < b.getId() : a.getId() > b.getId();
            if (sortCol == 1) return sortAscending ? a.getName() < b.getName() : a.getName() > b.getName();
            if (sortCol == 2) return sortAscending ? a.getPrice() < b.getPrice() : a.getPrice() > b.getPrice();
            if (sortCol == 3) return sortAscending ? a.getQuantity() < b.getQuantity() : a.getQuantity() > b.getQuantity();
            if (sortCol == 4) {
                std::string wa = productWarehouses.count(a.getId()) ? productWarehouses.at(a.getId()) : "";
                std::string wb = productWarehouses.count(b.getId()) ? productWarehouses.at(b.getId()) : "";
                return sortAscending ? wa < wb : wa > wb;
            }
            if (sortCol == 5) {
                std::string la = productLocations.count(a.getId()) ? productLocations.at(a.getId()) : "";
                std::string lb = productLocations.count(b.getId()) ? productLocations.at(b.getId()) : "";
                return sortAscending ? la < lb : la > lb;
            }
            return false;
        });

        std::string queryLower = filterQuery;
        std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);

        int rowIndex = 0;
        for (const auto& prod : products) {
            std::string idStr = std::to_string(prod.getId());
            std::string nameStr = prod.getName();
            std::string whStr = productWarehouses.count(prod.getId()) ? productWarehouses.at(prod.getId()) : "-";
            std::string locStr = productLocations.count(prod.getId()) ? productLocations.at(prod.getId()) : "-";

            std::string searchCombined = idStr + " " + nameStr + " " + whStr + " " + locStr;
            std::transform(searchCombined.begin(), searchCombined.end(), searchCombined.begin(), ::tolower);

            if (!queryLower.empty() && searchCombined.find(queryLower) == std::string::npos) {
                continue;
            }

            inventoryGrid->AppendRows(1);
            inventoryGrid->SetCellValue(rowIndex, 0, idStr);
            inventoryGrid->SetCellValue(rowIndex, 1, nameStr);
            inventoryGrid->SetCellValue(rowIndex, 2, wxString::Format("%.2f", prod.getPrice()));
            inventoryGrid->SetCellValue(rowIndex, 3, std::to_string(prod.getQuantity()));
            inventoryGrid->SetCellValue(rowIndex, 4, whStr);
            inventoryGrid->SetCellValue(rowIndex, 5, locStr);

            bool isLowStock = prod.getQuantity() <= 5;
            for (int col = 0; col < 6; ++col) {
                if (isLowStock) {
                    inventoryGrid->SetCellBackgroundColour(rowIndex, col, wxColour(80, 40, 45));
                    inventoryGrid->SetCellTextColour(rowIndex, col, wxColour(255, 128, 128));
                } else {
                    inventoryGrid->SetCellBackgroundColour(rowIndex, col, wxColour(40, 42, 54));
                    inventoryGrid->SetCellTextColour(rowIndex, col, wxColour(248, 248, 242));
                }
            }
            rowIndex++;
        }

        inventoryGrid->AutoSizeColumns();
        inventoryGrid->ForceRefresh();
    }

    void OnGridCellSelected(wxGridEvent& event) {
        int row = event.GetRow();
        if (row >= 0 && row < inventoryGrid->GetNumberRows()) {
            try {
                int productId = std::stoi(inventoryGrid->GetCellValue(row, 0).ToStdString());
                DisplayProductImage(productId);
            } catch (...) {}
        }
        event.Skip();
    }

    void OnGridLabelClick(wxGridEvent& event) {
        int col = event.GetCol();
        if (col >= 0 && col < 6) {
            if (sortCol == col) {
                sortAscending = !sortAscending;
            } else {
                sortCol = col;
                sortAscending = true;
            }
            PopulateGrid(searchCtrl->GetValue().ToStdString());
        }
        event.Skip();
    }

    void OnSearch(wxCommandEvent& event) {
        PopulateGrid(searchCtrl->GetValue().ToStdString());
    }

    void OnRefresh(wxCommandEvent& event) {
        searchCtrl->Clear();
        imagePreviewCtrl->SetBitmap(wxNullBitmap);
        PopulateGrid();
    }

    void OnAddProduct(wxCommandEvent& event) {
        AddProductDialog dlg(this);
        if (dlg.ShowModal() == wxID_OK) {
            try {
                int id = dlg.GetID();
                std::string name = dlg.GetProductName();
                double price = dlg.GetPrice();
                int qty = dlg.GetQuantity();
                std::string warehouse = dlg.GetWarehouse();
                std::string location = dlg.GetLocation();
                std::string imgPath = dlg.GetImagePath();

                if (name.empty()) {
                    wxMessageBox("Product Name cannot be empty.", "Validation Error", wxOK | wxICON_WARNING);
                    return;
                }

                for (const auto& prod : manager.getAllProducts()) {
                    if (prod.getId() == id) {
                        wxMessageBox("A product with ID " + std::to_string(id) + " already exists!",
                                     "Duplicate ID Error", wxOK | wxICON_WARNING);
                        return;
                    }
                }

                manager.addProduct(Product(id, name, price, qty));
                if (!imgPath.empty()) productImages[id] = imgPath;
                if (!warehouse.empty()) productWarehouses[id] = warehouse;
                if (!location.empty()) productLocations[id] = location;

                SaveData();
                PopulateGrid(searchCtrl->GetValue().ToStdString());
            } 
            catch (const std::exception&) {
                wxMessageBox("Invalid input! Please enter numeric values for ID, Price, and Quantity.", 
                             "Input Error", wxOK | wxICON_ERROR);
            }
        }
    }

    void OnUpdateStock(wxCommandEvent& event) {
        wxArrayInt selectedRows = inventoryGrid->GetSelectedRows();
        if (selectedRows.IsEmpty()) {
            wxMessageBox("Please click a row label on the left to select a product.", 
                         "Notice", wxOK | wxICON_INFORMATION);
            return;
        }

        int row = selectedRows[0];
        int productId = std::stoi(inventoryGrid->GetCellValue(row, 0).ToStdString());

        UpdateStockDialog dlg(this, std::stoi(inventoryGrid->GetCellValue(row, 3).ToStdString()));
        if (dlg.ShowModal() == wxID_OK) {
            try {
                int newQty = dlg.GetQuantity();
                
                auto products = manager.getAllProducts();
                for (const auto& p : products) {
                    if (p.getId() == productId) {
                        manager.removeProduct(productId);
                        manager.addProduct(Product(p.getId(), p.getName(), p.getPrice(), newQty));
                        break;
                    }
                }

                SaveData();
                PopulateGrid(searchCtrl->GetValue().ToStdString());
            }
            catch (const std::exception&) {
                wxMessageBox("Invalid quantity entered.", "Input Error", wxOK | wxICON_ERROR);
            }
        }
    }

    void OnDeleteProduct(wxCommandEvent& event) {
        wxArrayInt selectedRows = inventoryGrid->GetSelectedRows();
        if (selectedRows.IsEmpty()) {
            wxMessageBox("Please click a row label on the left to select a product to delete.", 
                         "Notice", wxOK | wxICON_INFORMATION);
            return;
        }

        int row = selectedRows[0];
        int productId = std::stoi(inventoryGrid->GetCellValue(row, 0).ToStdString());

        manager.removeProduct(productId);
        productImages.erase(productId);
        productWarehouses.erase(productId);
        productLocations.erase(productId);
        imagePreviewCtrl->SetBitmap(wxNullBitmap);

        SaveData();
        PopulateGrid(searchCtrl->GetValue().ToStdString());
    }

    void OnExportReport(wxCommandEvent& event) {
        wxFileDialog saveFileDialog(this, "Export Inventory Report", "", "inventory_report.txt",
                                    "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

        std::ofstream file(saveFileDialog.GetPath().ToStdString());
        if (!file.is_open()) {
            wxMessageBox("Failed to create report file.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        auto products = manager.getAllProducts();
        double totalValue = 0.0;
        int lowStockCount = 0;

        file << "===================================================================\n";
        file << "            INVENTORY MANAGEMENT SYSTEM REPORT (ZAR)               \n";
        file << "===================================================================\n\n";

        for (const auto& p : products) {
            double itemTotal = p.getPrice() * p.getQuantity();
            totalValue += itemTotal;
            if (p.getQuantity() <= 5) lowStockCount++;

            std::string wh = productWarehouses.count(p.getId()) ? productWarehouses.at(p.getId()) : "N/A";
            std::string loc = productLocations.count(p.getId()) ? productLocations.at(p.getId()) : "N/A";

            file << "ID: " << p.getId()
                 << " | Name: " << p.getName()
                 << " | Price: R " << std::fixed << std::setprecision(2) << p.getPrice()
                 << " | Qty: " << p.getQuantity()
                 << " | WH: " << wh
                 << " | Loc: " << loc
                 << (p.getQuantity() <= 5 ? " [LOW STOCK]" : "") << "\n";
        }

        file << "\n-------------------------------------------------------------------\n";
        file << "Total Unique Items: " << products.size() << "\n";
        file << "Total Inventory Valuation: R " << std::fixed << std::setprecision(2) << totalValue << "\n";
        file << "Low Stock Items Count: " << lowStockCount << "\n";
        file << "===================================================================\n";

        file.close();
        wxMessageBox("Inventory report exported successfully!", "Export Complete", wxOK | wxICON_INFORMATION);
    }

public:
    UIMainWindow(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1180, 720)) {
        
        wxColour bgColor(30, 30, 46);       
        wxColour panelBg(40, 42, 54);       
        wxColour textColor(248, 248, 242);    
        wxColour accentColor(139, 233, 253);  

        SetBackgroundColour(bgColor);

        wxPanel* panel = new wxPanel(this);
        panel->SetBackgroundColour(bgColor);
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        // Header Title
        wxStaticText* header = new wxStaticText(panel, wxID_ANY, "Inventory Management System");
        header->SetForegroundColour(accentColor);
        wxFont font = header->GetFont();
        font.SetPointSize(16);
        font.SetWeight(wxFONTWEIGHT_BOLD);
        header->SetFont(font);
        mainSizer->Add(header, 0, wxALL | wxALIGN_CENTER, 10);

        // Summary Statistics Banner
        summaryLabel = new wxStaticText(panel, wxID_ANY, "Total Products: 0 | Total Valuation: R 0.00 | Low Stock Alerts: 0");
        summaryLabel->SetForegroundColour(wxColour(80, 250, 123)); 
        wxFont summaryFont = summaryLabel->GetFont();
        summaryFont.SetWeight(wxFONTWEIGHT_BOLD);
        summaryLabel->SetFont(summaryFont);
        mainSizer->Add(summaryLabel, 0, wxALIGN_CENTER | wxBOTTOM, 10);

        // Search Bar Area
        wxBoxSizer* searchSizer = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText* searchLabel = new wxStaticText(panel, wxID_ANY, "Search Inventory:");
        searchLabel->SetForegroundColour(textColor);
        
        searchCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1));
        searchCtrl->SetHint("Type ID, Name, Warehouse, or Location...");

        searchSizer->Add(searchLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 8);
        searchSizer->Add(searchCtrl, 1, wxEXPAND);
        mainSizer->Add(searchSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

        // Central Layout Split: Grid + Image Preview
        wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);

        inventoryGrid = new wxGrid(panel, wxID_ANY);
        inventoryGrid->CreateGrid(0, 6);

        // Theme the Grid
        inventoryGrid->SetGridLineColour(wxColour(60, 62, 74));
        inventoryGrid->SetDefaultCellBackgroundColour(panelBg);
        inventoryGrid->SetDefaultCellTextColour(textColor);
        inventoryGrid->SetLabelBackgroundColour(wxColour(68, 71, 90));
        inventoryGrid->SetLabelTextColour(textColor);

        inventoryGrid->SetColLabelValue(0, wxString::FromUTF8("Product ID \xE2\x96\xB2\xE2\x96\xBC"));
        inventoryGrid->SetColLabelValue(1, wxString::FromUTF8("Name \xE2\x96\xB2\xE2\x96\xBC"));
        inventoryGrid->SetColLabelValue(2, wxString::FromUTF8("Price (R) \xE2\x96\xB2\xE2\x96\xBC"));
        inventoryGrid->SetColLabelValue(3, wxString::FromUTF8("Stock Quantity \xE2\x96\xB2\xE2\x96\xBC"));
        inventoryGrid->SetColLabelValue(4, wxString::FromUTF8("Warehouse \xE2\x96\xB2\xE2\x96\xBC"));
        inventoryGrid->SetColLabelValue(5, wxString::FromUTF8("Location \xE2\x96\xB2\xE2\x96\xBC"));
        inventoryGrid->EnableEditing(false);
        inventoryGrid->SetSelectionMode(wxGrid::wxGridSelectRows);

        contentSizer->Add(inventoryGrid, 1, wxEXPAND | wxRIGHT, 10);

        // Image Preview Side Card
        wxStaticBoxSizer* previewBox = new wxStaticBoxSizer(wxVERTICAL, panel, "Product Image Preview");
        previewBox->GetStaticBox()->SetForegroundColour(textColor);
        
        imagePreviewCtrl = new wxStaticBitmap(previewBox->GetStaticBox(), wxID_ANY, wxBitmap(200, 200));
        previewBox->Add(imagePreviewCtrl, 0, wxALIGN_CENTER | wxALL, 10);

        contentSizer->Add(previewBox, 0, wxEXPAND | wxLEFT, 5);
        mainSizer->Add(contentSizer, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

        // Control Toolbar with Colored Action Buttons
        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        
        wxButton* btnRefresh = new wxButton(panel, wxID_ANY, "Clear / Refresh");
        wxButton* btnAdd = new wxButton(panel, wxID_ANY, "Add Product");
        wxButton* btnUpdateStock = new wxButton(panel, wxID_ANY, "Update Stock");
        wxButton* btnDelete = new wxButton(panel, wxID_ANY, "Delete Selected");
        wxButton* btnExport = new wxButton(panel, wxID_ANY, "Export Report");

        btnAdd->SetBackgroundColour(wxColour(80, 250, 123));       
        btnAdd->SetForegroundColour(wxColour(40, 42, 54));

        btnUpdateStock->SetBackgroundColour(wxColour(241, 250, 140)); 
        btnUpdateStock->SetForegroundColour(wxColour(40, 42, 54));

        btnDelete->SetBackgroundColour(wxColour(255, 85, 85));     
        btnDelete->SetForegroundColour(wxColour(255, 255, 255));

        buttonSizer->Add(btnRefresh, 0, wxALL, 5);
        buttonSizer->Add(btnAdd, 0, wxALL, 5);
        buttonSizer->Add(btnUpdateStock, 0, wxALL, 5);
        buttonSizer->Add(btnDelete, 0, wxALL, 5);
        buttonSizer->Add(btnExport, 0, wxALL, 5);

        mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

        // Event Bindings
        searchCtrl->Bind(wxEVT_TEXT, &UIMainWindow::OnSearch, this);
        inventoryGrid->Bind(wxEVT_GRID_LABEL_LEFT_CLICK, &UIMainWindow::OnGridLabelClick, this);
        inventoryGrid->Bind(wxEVT_GRID_SELECT_CELL, &UIMainWindow::OnGridCellSelected, this);
        btnRefresh->Bind(wxEVT_BUTTON, &UIMainWindow::OnRefresh, this);
        btnAdd->Bind(wxEVT_BUTTON, &UIMainWindow::OnAddProduct, this);
        btnUpdateStock->Bind(wxEVT_BUTTON, &UIMainWindow::OnUpdateStock, this);
        btnDelete->Bind(wxEVT_BUTTON, &UIMainWindow::OnDeleteProduct, this);
        btnExport->Bind(wxEVT_BUTTON, &UIMainWindow::OnExportReport, this);

        panel->SetSizer(mainSizer);

        LoadData();
        PopulateGrid();
    }
};

class InventoryApp : public wxApp {
public:
    virtual bool OnInit() {
        wxInitAllImageHandlers();
        UIMainWindow* window = new UIMainWindow("Inventory Management System");
        window->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(InventoryApp);