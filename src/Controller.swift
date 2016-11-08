// azul
// Copyright © 2016 Ken Arroyo Ohori
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

import Metal
import MetalKit

@NSApplicationMain
class Controller: NSObject, NSApplicationDelegate {
  
  @IBOutlet weak var window: NSWindow!
  @IBOutlet weak var splitView: NSSplitView!
  @IBOutlet weak var outlineView: NSOutlineView!
  var view: NSView?
  @IBOutlet weak var progressIndicator: NSProgressIndicator!
  
  @IBOutlet weak var toggleViewEdgesMenuItem: NSMenuItem!
  @IBOutlet weak var toggleViewBoundingBoxMenuItem: NSMenuItem!
  @IBOutlet weak var goHomeMenuItem: NSMenuItem!
  @IBOutlet weak var toggleSideBarMenuItem: NSMenuItem!
  
  let dataStorage = DataStorage()
  
  func applicationDidFinishLaunching(_ aNotification: Notification) {
    Swift.print("AppDelegate.applicationDidFinishLaunching(Notification)")
    
    if let defaultDevice = MTLCreateSystemDefaultDevice() {
      let metalView = MetalView(frame: splitView.subviews[1].frame, device: defaultDevice)
      dataStorage.metalView = metalView
      metalView.controller = self
      metalView.dataStorage = dataStorage
      metalView.subviews = splitView.subviews[1].subviews
      
      splitView.removeArrangedSubview(splitView.arrangedSubviews[1])
      splitView.insertArrangedSubview(metalView, at: 1)
      
//      splitView.subviews.append(metalView)
//      metalView.subviews = splitView.subviews[1].subviews
//      splitView.subviews[1] = metalView
//      Swift.print("View \(splitView.subviews[1])")
    } else {
      
      return
    }
    
    dataStorage.controller = self
    outlineView.delegate = dataStorage
    outlineView.dataSource = dataStorage
    outlineView.doubleAction = #selector(outlineViewDoubleClick)
    toggleSideBar(toggleSideBarMenuItem)
  }
  
  func application(_ sender: NSApplication, openFile filename: String) -> Bool {
    Swift.print("AppDelegate.application(NSApplication, openFile: String)")
    Swift.print("Open \(filename)")
    let url = URL(fileURLWithPath: filename)
    self.dataStorage.loadData(from: [url])
    return true
  }
  
  func application(_ sender: NSApplication, openFiles filenames: [String]) {
    Swift.print("AppDelegate.application(NSApplication, openFiles: String)")
    Swift.print("Open \(filenames)")
    var urls = [URL]()
    for filename in filenames {
      urls.append(URL(fileURLWithPath: filename))
      
    }
    self.dataStorage.loadData(from: urls)
  }
  
  func outlineViewDoubleClick(_ sender: Any?) {
    dataStorage.outlineViewDoubleClick(sender)
  }
  
  func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool {
    return true
  }
  
  @IBAction func new(_ sender: NSMenuItem) {
    Swift.print("Controller.new(NSMenuItem)")
    
    dataStorage.openFiles = Set<URL>()
    self.window.representedURL = nil
    self.window.title = "Azul"
    
    dataStorage.objects.removeAll()
    self.outlineView.reloadData()
    
    if let metalView = view as? MetalView {
      metalView.new()
    } else {
      
    }
  }
  
  @IBAction func openFile(_ sender: NSMenuItem) {
    Swift.print("AppDelegate.openFile(NSMenuItem)")
    
    let openPanel = NSOpenPanel()
    openPanel.allowsMultipleSelection = true
    openPanel.canChooseDirectories = false
    openPanel.canChooseFiles = true
    openPanel.allowedFileTypes = ["gml", "xml"]
    openPanel.begin(completionHandler:{(result: Int) in
      if result == NSFileHandlingPanelOKButton {
        self.dataStorage.loadData(from: openPanel.urls)
      }
    })
  }
  
  @IBAction func toggleViewEdges(_ sender: NSMenuItem) {
    if let metalView = view as? MetalView {
      if metalView.viewEdges {
        metalView.viewEdges = false
        sender.state = 0
        metalView.needsDisplay = true
      } else {
        metalView.viewEdges = true
        sender.state = 1
        metalView.needsDisplay = true
      }
    } else {
      
    }
  }
  
  @IBAction func toggleViewBoundingBox(_ sender: NSMenuItem) {
    if let metalView = view as? MetalView {
      if metalView.viewBoundingBox {
        metalView.viewBoundingBox = false
        sender.state = 0
        metalView.needsDisplay = true
      } else {
        metalView.viewBoundingBox = true
        sender.state = 1
        metalView.needsDisplay = true
      }
    } else {
      
    }
  }
  
  @IBAction func goHome(_ sender: NSMenuItem) {
    if let metalView = view as? MetalView {
      metalView.goHome()
    } else {
      
    }
  }
  
  @IBAction func toggleSideBar(_ sender: NSMenuItem) {
    if splitView.subviews[0].bounds.size.width == 0 {
      //      Swift.print("Open sidebar")
      splitView.setPosition(200, ofDividerAt: 0)
      sender.title = "Hide Sidebar"
    } else {
      //      Swift.print("Close sidebar")
      splitView.setPosition(0, ofDividerAt: 0)
      sender.title = "Show Sidebar"
    }
  }
  
  func applicationWillTerminate(_ aNotification: Notification) {
    Swift.print("AppDelegate.applicationWillTerminate()")
  }
  
}
