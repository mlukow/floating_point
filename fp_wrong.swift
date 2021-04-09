import Metal

func test(device: MTLDevice) {
    print("Testing on \(device.name)")

    let url = URL(fileURLWithPath: "library.metallib")
    guard let library = try? device.makeLibrary(URL: url) else {
        return print("Could not create library")
    }

    guard let function = library.makeFunction(name: "iter") else {
        return print("Could not create function")
    }

    do {
        let state = try device.makeComputePipelineState(function: function)

        guard let queue = device.makeCommandQueue() else {
            return print("Could not create queue")
        }

        guard let commandBuffer = queue.makeCommandBuffer() else {
            return print("Could not create command buffer")
        }

        guard let commandEncoder = commandBuffer.makeComputeCommandEncoder() else {
            return print("Could not create compute command encoder")
        }

        commandEncoder.setComputePipelineState(state)
        
        var data = [Float](repeating: 0, count: 32)
        data[0] = 2
        data[1] = -4
        guard let input = device.makeBuffer(bytes: &data, length: data.count * MemoryLayout<Float>.size, options: .storageModeShared) else {
            return print("Could not create buffer")
        }

        commandEncoder.setBuffer(input, offset: 0, index: 0)

        let size = MTLSizeMake(1, 1, 1)
        commandEncoder.dispatchThreads(MTLSizeMake(32, 1, 1), threadsPerThreadgroup: size)

        commandEncoder.endEncoding()
        commandBuffer.commit()
        commandBuffer.waitUntilCompleted()

        let nsData = NSData(bytesNoCopy: input.contents(), length: data.count * MemoryLayout<Float>.size, freeWhenDone: false)
        data = [Float].init(repeating: 0, count: data.count)
        nsData.getBytes(&data, length:data.count * MemoryLayout<Float>.size)

        print("\(data[data.count - 1])")
    } catch {
        return print("Could not create state: \(error)")
    }
}

MTLCopyAllDevices().forEach { device in
    test(device: device)
}
